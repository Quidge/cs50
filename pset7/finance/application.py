import sqlite3
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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
    """Show portfolio of stocks"""
    return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    return apology("TODO")


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

        # Query database for username
        c.execute("SELECT * FROM users WHERE username=?",
                    (username,))
        rows = c.fetchall()

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0][2], ptpass):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0][0]

        print(session["user_id"])

        # Redirect user to home page
        return redirect("/")

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
    return apology("TODO")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    error = None

    if request.method == "POST":

        username = request.form.get("username")
        ptpass = request.form.get("password")

        print(f"username:{username}, password:{ptpass}")

        if not request.form.get("username") or not request.form.get("password") or not request.form.get("confirmation"):
            error = "Missing fields"
        elif request.form.get("password") != request.form.get("confirmation"):
            error = "Passwords do not match"

        # connect to db

        c.execute("SELECT * FROM users WHERE username=?", (username,))
        rows = c.fetchall()

        if len(rows) > 1:
            error = "Wha? There are {num} users with that \
                    username".format(num=len(rows))
        elif len(rows) > 0:
            error = "A user with the name, '{username}', already exists.".format(username=username)
        else:
            hashpass = generate_password_hash(ptpass, method="md5")
            c.execute("INSERT INTO users(username, hash) VALUES (?, ?)", (username, hashpass,))
            db.commit()
            flash("Account created successfully!")
            user = c.execute("SELECT id FROM users WHERE username=?",
                            (username,))
            row = user.fetchone()
            session["user_id"] = row[0]
            return redirect("/")
    print(error)
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
