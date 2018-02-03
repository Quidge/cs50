import csv
import urllib.request
import sqlite3
import pickle

from flask import redirect, render_template, request, session
from functools import wraps
from werkzeug.security import check_password_hash, generate_password_hash

def apology(message, code=400):
    """Renders message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code

def valid_login(username, plaintext, database):
    '''
    Returns true if username/password combination is in database.
    '''

    c = database.cursor()
    c.execute("SELECT * FROM users WHERE username=?", (username,))
    row = c.fetchone()

    if check_password_hash(row[2], plaintext):
        return True

    return False

def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.12/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # reject symbol if it starts with caret
    if symbol.startswith("^"):
        return None

    # Reject symbol if it contains comma
    if "," in symbol:
        return None

    # Query Yahoo for quote
    # http://stackoverflow.com/a/21351911
    try:

        # GET CSV
        url = f"http://download.finance.yahoo.com/d/quotes.csv?f=snl1&s={symbol}"
        webpage = urllib.request.urlopen(url)

        # Read CSV
        datareader = csv.reader(webpage.read().decode("utf-8").splitlines())

        # Parse first row
        row = next(datareader)

        # Ensure stock exists
        try:
            price = float(row[2])
        except:
            return None

        # Return stock's name (as a str), price (as a float), and (uppercased) symbol (as a str)
        return {
            "name": row[1],
            "price": price,
            "symbol": row[0].upper(),
            "source": "Yahoo Finance"
        }

    except:
        pass

    # Query Alpha Vantage for quote instead
    # https://www.alphavantage.co/documentation/
    try:

        # GET CSV
        url = f"https://www.alphavantage.co/query?apikey=NAJXWIA8D6VN6A3K&datatype=csv&function=TIME_SERIES_INTRADAY&interval=1min&symbol={symbol}"
        webpage = urllib.request.urlopen(url)

        # Parse CSV
        datareader = csv.reader(webpage.read().decode("utf-8").splitlines())

        # Ignore first row
        next(datareader)

        # Parse second row
        row = next(datareader)

        # Ensure stock exists
        try:
            price = float(row[4])
        except:
            return None

        # Return stock's name (as a str), price (as a float), and (uppercased) symbol (as a str)
        return {
            "name": symbol.upper(),  # for backward compatibility with Yahoo
            "price": price,
            "symbol": symbol.upper(),
            "source": "AlphaVantage"
        }

    except:
        return None


def usd(value):
    """Formats value as USD."""
    return f"${value:,.2f}"

def sell_stock(user_id, symbol, share_price, num_shares):
    """Creates SELL transaction in transaction table. Credits corresponding
        amount to users cash. Assumes ONLY stock symbol, stock price, and user_id
        are accurate and will return None if other parameters fail validation.

        Return -1 if user does not have stock in portfolio.
        Return 0 if user does not have enough shares to complete desired sale.
        Returns user's cash balance after sale transaction."""

    assert num_shares > 0, "Number of shares is 0 or negative."

    db = sqlite3.connect('finance.db')
    c = db.cursor()

    # gather records of any BUY transaction made by user
    # return -1 if none found - user has never purchased $SYMBOL
    c.execute("SELECT COUNT(num_shares) \
                FROM transactions \
                WHERE user_id=? AND stock_symbol=? AND operation=?",
                (user_id, symbol, 'BUY',))
    try:
        shares_bought = c.fetchone()[0]
    except IndexError:
        return -1

    c.execute("SELECT COUNT(num_shares) \
                FROM transactions \
                WHERE user_id=? AND stock_symbol=? AND operation=?",
                (user_id, symbol, 'SELL',))
    try:
        shares_sold = c.fetchone()[0]
    except IndexError:
        shares_sold = 0

    if num_shares >= shares_bought - shares_sold:
        try:
            c.execute("INSERT INTO transactions (stock_symbol, num_shares, \
                                                share_price, operation, \
                                                user_id) \
                        VALUES (?, ?, ?, ?, ?)",
                        (symbol, num_shares, share_price, 'SELL', user_id,))
            c.execute("SELECT cash FROM users WHERE id=?", (user_id,))
            pre_sale_cash = c.fetchone()[0]
            sale_gross = num_shares * share_price;
            post_sale_cash = pre_sale_cash + sale_gross
            c.execute("UPDATE users SET cash=? WHERE id=?", (post_sale_cash, user_id,))
            db.commit() # i don't understand when/when not to commit
            return post_sale_cash
        except sqlite3.OperationalError as e:
            print(e)
            raise
    else:
        return 0


def buy_stock(user_id, symbol, share_price, num_shares):
    """Creates BUY transaction in transaction table. Debits corresponding
        amount from users cash. Assumes ONLY stock symbol and stock price are
        accurate and will return None if other parameters fail validation.

        Returns user's cash balance minus total purchase cost."""

    db = sqlite3.connect('finance.db')
    c = db.cursor()
    c.execute("SELECT cash FROM users WHERE ID=?", (user_id,))
    user_cash = c.fetchone()
    print(user_cash)

    # validate user is real
    try:
        assert user_cash != None
    except AssertionError:
        print("User '{user_id}' does not exist")
        return None

    try:
        user_cash = user_cash[0]
    except IndexError:
        print ("Can't find index 0 of user_cash")
        return None

    try:
        assert num_shares > 0
    except AssertionError:
        print("Number of shares is invalid")
        return None

    # validate user has enough cash on hand to make purchase
    try:
        assert user_cash >= num_shares * share_price
        c.execute("UPDATE users SET cash=? WHERE id=?",
                    (user_cash - (share_price * num_shares), user_id,))
        c.execute("INSERT INTO transactions ( \
                                            stock_symbol, num_shares, \
                                            share_price, user_id, \
                                            operation) \
                                VALUES (?, ?, ?, ?, ?)",
                                (symbol, num_shares, share_price, user_id, 'BUY',))
        db.commit()
    except AssertionError:
        raise ArithmeticError("Insufficient balance for purchase.")

    # query new cash amount for user and return that new balance
    c.execute("SELECT cash FROM users WHERE ID=?", (user_id,))
    return c.fetchone()[0]

def store_snapshot(user_id, data_table, generation_date):
    """Stores a pickled snapshot of user portfolio to user_portfolio_snapshots.

        If entry already exists in user_portfolio_snapshots for user_id, then
        the old entry is overwritten.

        Returns True if sucessful and False if unsucessful.
        """

    try:
        assert user_id and data_table and generation_date
    except AssertionError as e:
        print(e)
        return False

    db = sqlite3.connect('finance.db')
    c = db.cursor()
    data_string = pickle.dumps(data_table)

    c.execute("SELECT * FROM user_portfolio_snapshots WHERE user_id=?", (user_id,))

    row = c.fetchone()

    try:
        if len(row) > 0:
            c.execute("UPDATE user_portfolio_snapshots \
                        SET data=?, generation_date=? \
                        WHERE user_id=?", (data_string, generation_date, user_id,))
        else:
            c.execute("INSERT INTO user_portfolio_snapshots (user_id, generation_date, data) \
                        VALUES (?, ?, ?)", (user_id, generation_date, data_string,))
        db.commit()
    except sqlite3.OperationalError as e:
        print(e)
        return False

    return True


