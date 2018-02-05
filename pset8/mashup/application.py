import os
import re
import sqlite3
from flask import Flask, jsonify, render_template, request

from helpers import lookup

# Configure application
app = Flask(__name__)

db = sqlite3.connect('mashup.db')


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
def index():
    """Render map"""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))


@app.route("/articles")
def articles():
    """Look up articles for geo"""

    geo = request.args.get("geo")

    try:
        assert geo != None
    except AssertionError:
        raise AttributeError("Geographic parameter not provided.")

    return jsonify(lookup(geo))


@app.route("/search", methods=["GET"])
def search():
    """Search for places that match query

    Only supports zip codes right now.
    """


    postal_code = request.args.get("q")

    #validation
    try:
        assert postal_code, "No search query parameter provided"
        assert len(postal_code) == 5, "Invalid zip code as query parameter"
        postal_code = int(postal_code)
    except AssertionError as e:
        print(f"{type(e).__name__}: {e}")
        raise
    except ValueError as e:
        print(f"{type(e).__name__}: {e}")
        raise

    # Search the database for matching zip codes
    c = db.cursor()
    c.execute("SELECT * FROM places WHERE postal_code=?", (postal_code,))
    results = c.fetchall()

    """query = [entry.strip() for entry in request.args.get("q").split(",")]

        Idea is to locate zip code in string. If zip is in string, apply that.
            > Dismiss other terms and return list from there (rationale: zip
            code won't result in more than 10 results; I know this is lazy)
        If no zip present, take any two letter terms and apply them against state field."""

    #return jsonify(results)
    return None


@app.route("/update")
def update():
    """Find up to 10 places within view"""

    # Ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # Ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # Explode southwest corner into two variables
    sw_lat, sw_lng = map(float, request.args.get("sw").split(","))

    # Explode northeast corner into two variables
    ne_lat, ne_lng = map(float, request.args.get("ne").split(","))

    # grab database cursor
    c = db.cursor()

    # init var before giant ass check
    rows = []

    # Find 10 cities within view, pseudorandomly chosen if more within view
    if sw_lng <= ne_lng:

        # Doesn't cross the antimeridian
        #rows = db.execute("""SELECT * FROM places
        #                  WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
        #                  GROUP BY country_code, place_name, admin_code1
        #                  ORDER BY RANDOM()
        #                  LIMIT 10""",
        #                  sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)
        c.execute("""SELECT * FROM places
                  WHERE ? <= latitude AND latitude <= ? AND (? <= longitude AND longitude <= ?)
                  GROUP BY country_code, place_name, admin_code1
                  ORDER BY RANDOM()
                  LIMIT 10""",
                  (sw_lat, ne_lat, sw_lng, ne_lng,))

    else:

        # Crosses the antimeridian
        #rows = db.execute("""SELECT * FROM places
        #                  WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
        #                  GROUP BY country_code, place_name, admin_code1
        #                  ORDER BY RANDOM()
        #                  LIMIT 10""",
        #                  sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)
        c.execute("""SELECT * FROM places
                  WHERE ? <= latitude AND latitude <= ? AND (? <= longitude OR longitude <= ?)
                  GROUP BY country_code, place_name, admin_code1
                  ORDER BY RANDOM()
                  LIMIT 10""",
                  (sw_lat, ne_lat, sw_lng, ne_lng,))

    rows = c.fetchall()

    # Output places as JSON
    return jsonify(rows)
