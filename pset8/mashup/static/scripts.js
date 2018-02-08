// Google Map
let map;

// Markers for map
let markers = [];

// Info window
let info = new google.maps.InfoWindow();


// Execute when the DOM is fully loaded
$(document).ready(function() {

    // Styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    let styles = [

        // Hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },

        // Hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "off"}
            ]
        }

    ];

    // Options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    let options = {
        center: {lat: 42.3770, lng: -71.1256}, // Stanford, California
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // Get DOM node in which map will be instantiated
    let canvas = $("#map-canvas").get(0);

    // Instantiate map
    map = new google.maps.Map(canvas, options);

    // Configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);

});


// Add marker for place to map
function addMarker(place)
{
    // construct marker with coords
    var marker = new google.maps.Marker({
        position: {lat: place[9], lng: place[10]},
        title: place[2] + ", " + place[3]
    });

    // add to marker array
    markers.push(marker);

    // add marker to map
    marker.setMap(map);

    marker.addListener('click', function() {
        showInfoPromise(marker, $.getJSON("/articles", {geo: place[1]}));
    });
}


// Configure application
function configure()
{
    // Update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {

        // If info window isn't open
        // http://stackoverflow.com/a/12410385
        if (!info.getMap || !info.getMap())
        {
            update();
        }
    });

    // Update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // Configure typeahead
    $("#q").typeahead({
        highlight: false,
        minLength: 1
    },
    {
        display: function(suggestion) {
            return null;
        },
        limit: 10,
        source: search,
        templates: {
            suggestion: function(row) {
                return "<div>"+row[2]+", "+row[4]+", "+row[1]+"</div>";
            }
        }
    });

    // Re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // Set map's center
        map.setCenter({lat: parseFloat(suggestion[9]), lng: parseFloat(suggestion[10])});

        // Update UI
        update();
    });

    // Hide info window when text box has focus
    $("#q").focus(function(eventData) {
        info.close();
    });

    // Re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true;
        event.stopPropagation && event.stopPropagation();
        event.cancelBubble && event.cancelBubble();
    }, true);

    // Update UI
    update();

    // Give focus to text box
    $("#q").focus();
}


// Remove markers from map
function removeMarkers()
{
    markers.forEach(function(e) {
        e.setMap(null);
        e = null;
    })
    markers.length = 0;
}


// Search database for typeahead's suggestions
function search(query, syncResults, asyncResults)
{
    // Get places matching query (asynchronously)
    let parameters = {
        q: query
    };
    $.getJSON("/search", parameters, function(data, textStatus, jqXHR) {
        asyncResults(data);
    });
}

function showInfoPromise(marker, promisedContent) {
    // expects promisedContent to be a promise of JSON info

    // synch
    let loadingDiv = "<div id='info'><img alt='loading' "
                + "src='/static/ajax-loader.gif' />"
                + "</div>";
    info.setContent(loadingDiv);

    // asynch
    promisedContent
        .done(function(data) {

            let contentDiv = "<div id='info'>";

            // create unordered list and append to info div
            htmlString = "<ul>";
            var counter = 0;
            for (var article of data) {
                if (counter > 9) break;
                htmlString +=
                    "\n\t<li>"
                        +"\n\t\t<a href="+article["link"]+">"+article["title"]+"</a>"
                    +"\n\t</li>"
                counter++;
            };

            htmlString += "</ul>";
            contentDiv += htmlString;
            contentDiv += "</div>";
            info.setContent(contentDiv);
            info.open(map, marker);
        });

    // Open info window (if not already open)
    info.open(map, marker);
}

// Update UI's markers
function update()
{
    // Get map's bounds
    let bounds = map.getBounds();
    let ne = bounds.getNorthEast();
    let sw = bounds.getSouthWest();

    // Get places within bounds (asynchronously)
    let parameters = {
        ne: `${ne.lat()},${ne.lng()}`,
        q: $("#q").val(),
        sw: `${sw.lat()},${sw.lng()}`
    };
    $.getJSON("/update", parameters, function(data, textStatus, jqXHR) {

       // Remove old markers from map
       removeMarkers();

       // Add new markers to map
       for (let i = 0; i < data.length; i++)
       {
           addMarker(data[i]);
       }
    });
};
