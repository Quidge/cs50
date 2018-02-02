import sqlite3
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, valid_login, buy_stock

# Configure application
app = Flask(__name__)

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
#db = SQL("sqlite:///finance.db")

# Using sqlite3 to connect to db
db = sqlite3.connect('finance.db')
c = db.cursor()

@app.route("/")
@login_required
def index():

    # Consider adding a REFRESH button on page. Refresh gets new info (and is a POST request)
    #   GET requests only show cached value
    #   Otherwise, cache the value and only show that. Consider holding cache in DB?

    try:
        c.execute("SELECT SUM(num_shares), stock_symbol \
                    FROM transactions \
                    WHERE user_id=? AND operation='BUY' \
                    GROUP BY stock_symbol", (session["user_id"],)
                    )
    except (sqlite3.ProgrammingError, sqlite3.OperationalError) as e:
        flash("{type}: {e}".format(type=type(e).__name__, e=e))
        print(e)

    db_rows = c.fetchall()

    # add current day price data to holdings queried from database
    #   shouldin't run if user doesn't own anything
    # THIS IS EXPENSIVE ON PAGELOAD
    rows_w_share_prices = [[value for value in row] for row in db_rows]
    for row in rows_w_share_prices:
        share_price = lookup(row[1])["price"]
        row.append(share_price)

    # get cash on hand
    c.execute("SELECT cash FROM users WHERE ID=?", (session["user_id"],))
    cash = c.fetchone()[0]

    # render template with info, if portfolio is empty skip and give defaults
    if len(rows_w_share_prices) > 0:
        # sum total portfolio value
        portfolio_sum = sum(row[0]*row[2] for row in rows_w_share_prices)

        return render_template("index.html", portfolio=rows_w_share_prices,
                                cash=cash, portfolio_sum=portfolio_sum)
    else:
        return render_template("index.html", portfolio=None, cash=cash, portfolio_sum=0)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "POST":
        symbol = request.form.get("symbol")
        num_shares = request.form.get("num_shares")
        stock_data = lookup(symbol)

        try:
            assert stock_data != None
            print(stock_data)
        except AssertionError:
            flash("Stock '{symbol}' doesn't seem to exist".format(symbol=symbol))
            return render_template("buy.html")

        try:
            num_shares = int(num_shares)
        except ValueError as e:
            print(e)
            print("Can't convert num_shares to int")
            flash("Use the fields properly dumbass.")
            return render_template("buy.html")

        try:
            result = buy_stock(session["user_id"], stock_data["symbol"],
                                stock_data["price"], num_shares)
            assert result != None
        except (AssertionError, ArithmeticError,) as e:
            flash("{type}: {e}".format(type=type(e).__name__, e=e))
            return render_template("buy.html")

        flash("Purchase successful! New cash balance: {result}".format(result=result))

    return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        username = request.form.get("username")
        ptpass = request.form.get("password")

        if valid_login(username, ptpass, db):

            # Query database for user info
            c.execute("SELECT * FROM users WHERE username=?",
                        (username,))
            row = c.fetchone()

            # Remember which user has logged in
            session["user_id"] = row[0]
            session["username"] = row[1]

            print(session["user_id"])

            # Redirect user to home page
            return redirect("/")
        else:
            return apology("invalid username and/or password", 403)
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        result = lookup(request.form.get("symbol"))
        if result == None:
            flash("Couldn't find that symbol, sorry!")
            return render_template("quote.html")
        else:
            return render_template("quote.html", stock=result)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # knowing i'm shit at session management, i'm saying clear it with any
    # access at all
    # and yes, that means if if a user accesses /register.html in any way,
    # they will need to log in again
    session.clear()

    error = None

    if request.method == "POST":

        if not request.form.get("username") or not request.form.get("password") or not request.form.get("confirmation"):
            error = "Missing fields"
            flash(error)
            return render_template("register.html", error=error)
        elif request.form.get("password") != request.form.get("confirmation"):
            error = "Passwords do not match"
            flash(error)
            return render_template("register.html", error=error)

        # if this logic passes, all fields must be filled in and
        # password matches confirmation

        # for brevity
        username = request.form.get("username")
        ptpass = request.form.get("password")

        # connect to db

        c.execute("SELECT * FROM users WHERE username=?", (username,))
        rows = c.fetchall()

        if len(rows) > 1:
            error = "Wha? There are {num} users with that \
                    username".format(num=len(rows))
        elif len(rows) > 0:
            error = "A user with the name, '{username}', already exists.".format(username=username)
        else:
            # everything passes, so create a new user:
            hashpass = generate_password_hash(ptpass, method="md5")
            c.execute("INSERT INTO users(username, hash) VALUES (?, ?)", (username, hashpass,))
            db.commit()
            flash("Account created successfully!")

            # set session vars to proper values
            user = c.execute("SELECT * FROM users WHERE username=?",
                            (username,))
            row = user.fetchone()
            session["user_id"] = row[0]
            session["username"] = username
            return redirect("/")

    if error is not None:
        print(error)
        flash(error)

    return render_template("register.html", error=error)


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
