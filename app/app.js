var temperature_chart = null;
var humidity_chart = null;
var light_chart = null;

var temperature_histo_chart = null;
var humidity_histo_chart = null;
var light_histo_chart = null;

var temperature_data = {
    chart: {
        subcaptionFontBold: 0,
        thmBulbRadius: 12,
        lowerLimit: -5,
        upperLimit: 40,
        numberSuffix: "°C",
        bgColor: "#ffffff",
        thmFillColor: "#008ee4",
        showBorder: 0,
        showValue: 0,
        showTickMarks: 0,
        showTickValues: 0
    },
    value: 0
};

var humidity_data = {
    chart: {
        numbersuffix: '%',
        lowerLimit: '0',
        upperLimit: '100',
        gaugeFillMix: '{dark-40},{light-40},{dark-20}',
        gaugeFillRatio: '85',
        gaugeOuterRadius: 50,
        gaugeInnerRadius: 45,
        pivotRadius: '10',
        showValue: '1',
        majorTMNumber: '8',
        minorTMNumber: '2',
        gaugeStartAngle: '155',
        gaugeEndAngle: '25',
        showTickValues: 0,
        theme: 'fint'
    },
    dials: {
        dial: [{
            bgColor: '#666666,#FFFFFF,#666666',
        showValue: 0,
            value: 0
        }]
    }
};

var light_data = {
    chart: {
        manageresize: "1",
        bgcolor: "ffffff",
        decimals: "0",
        numbersuffix: "%",
        lowerlimit: 0,
        upperlimit: 100,
        showborder: 0,
        showValue: 0,
        is3D: 1,
        placeValuesInside: 1,
        gaugeRadius: 0
    },
    colorrange: {
        color: [{
            minvalue: 0,
            maxvalue: 10,
            code: "808080"
        }, {
            minvalue: 10,
            maxvalue: 100,
            code: "F0F0A0"
        }]
    },
    value: 50
};


$(document).ready(function() {
    var width = Math.trunc($('#panel').width() / 3);

    temperature_chart = new FusionCharts({
        type: "thermometer",
        renderAt: "elem-temperature",
        width: width,
        height: 100,
        dataFormat: "json",
        dataSource: temperature_data
    });
    temperature_chart.render();

    humidity_chart = new FusionCharts({
        type: 'angulargauge',
        renderAt: 'elem-humidity',
        width: width,
        height: 120,
        dataFormat: 'json',
        dataSource: humidity_data
    });
    humidity_chart.render();

    light_chart = new FusionCharts({
        type: "bulb",
        renderAt: "elem-light",
        width: width,
        height: 120,
        dataFormat: "json",
        dataSource: light_data
    });
    light_chart.render();

    update();
});


function update() {
    $.getJSON("api.php", function(data) {
        console.log(data);

        temperature_data.value = data.temperature;
        humidity_data.dials.dial[0].value = data.humidity;
        light_data.value = data.light;
        light_data.chart.gaugeRadius = data.light / 2;

        temperature_chart.setJSONData(temperature_data);
        humidity_chart.setJSONData(humidity_data);
        light_chart.setJSONData(light_data);

        $("#elem-date").html(data.date);
        $("#elem-time").html(data.time);

        $("#elem-temperature-value").html(data.temperature+"°C");
        $("#elem-humidity-value").html(data.humidity+"%");
        $("#elem-light-value").html(data.light+"%");

        if (false) {
            // reanimate the change
            temperature_chart.render();
            humidity_chart.render();
            light_chart.render();
        }
        $("#readings").show();
    });
}


setInterval(update, 20 * 1000);     // update each 20 seconds
