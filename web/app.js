var date = null;        // current date


var temperature_chart = null;
var humidity_chart = null;
var light_chart = null;

var temperature_data = {
    chart: {
        subcaptionFontBold: 0,
        thmBulbRadius: 12,
        lowerLimit: -5,
        upperLimit: 40,
        numberSuffix: "°C",
        bgColor: "#ffffff",
        thmFillColor: "#ff2020",
        showBorder: 0,
        showValue: 0,
        showTickMarks: 0,
        showTickValues: 0
    },
    value: 0
};

var humidity_data = {
    chart: {
        numbersuffix: "%",
        lowerLimit: "0",
        upperLimit: "100",
        gaugeFillMix: "{dark-40},{light-40},{dark-20}",
        gaugeFillRatio: "85",
        gaugeOuterRadius: 70,
        gaugeInnerRadius: 40,
        pivotRadius: "10",
        showValue: "1",
        majorTMNumber: "8",
        minorTMNumber: "2",
        gaugeStartAngle: 135,
        gaugeEndAngle: 45,
        showTickValues: 0,
        theme: "fint"
    },
    dials: {
        dial: [{
            bgColor: "#666666,#FFFFFF,#666666",
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
    value: 0
};



function showReadings(readings) {
    var temperature_histo = [];
    var humidity_histo = [];
    var light_histo = [];

    var c = 0;
    for (reading of readings) {
        var dt = new Date(reading.moment).getTime();
        if (reading.temperature != 255) temperature_histo.push([dt, reading.temperature]);
        if (reading.humidity != 255) humidity_histo.push([dt, reading.humidity]);
        if (reading.light != 255) light_histo.push([dt, lux(reading.light)]);
    }

    var temperature_conf = [
        {
            data: temperature_histo,
            color: "red"
        }
    ];
    var humidity_conf = [
        {
            data: humidity_histo,
            color: "blue"
        }
    ];
    var light_conf = [
        {
            data: light_histo,
            color: "green"
        }
    ];

    var ops = {
        xaxis: {
            mode: "time",
            timezone: "browser"
        }
    };

    $.plot($("#temperature-histo"), temperature_conf, ops);
    $.plot($("#humidity-histo"), humidity_conf, ops);
    $.plot($("#light-histo"), light_conf, ops);
}



$(document).ready(function() {
    var width = Math.trunc($("#readings").width() / 3);

    // sets same height for all equal classed panels
    $('.equal .panel').matchHeight();

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
        type: "angulargauge",
        renderAt: "elem-humidity",
        width: width,
        height: 120,
        dataFormat: "json",
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

    $('#calendar').datetimepicker({
        inline: true,
        sideBySide: false,
        format: "YYYY-MM-dd",
        locale: "ca"
    })
    .on('dp.change',function(e) {
        var d = moment(new Date(e.date._d)).format("YYYY-MM-DD");
        changeDate(d);
    });

    UpdateDates();
    UpdateReadings();
});


function lux(x) {
    return Math.round(x / 7.0);
}


function changeDate(d) {
    date = d;
    $('.mydate').html(date);
    $('#panel-now').hide();
    $('#button-today').show();
    $.getJSON("/api/casa/dates/"+date, function(readings) {
        showReadings(readings);
    });


}


function UpdateDates() {
    $.getJSON("/api/casa/dates", function(dates) {
        ds = [];
        for (d of dates) ds.push(moment(d));

        $('#calendar').data("DateTimePicker").options({
            enabledDates: ds
        });
    });
}



function UpdateReadings () {
    if (date) return;

    $.getJSON("/api/casa", function(data) {

        data.light = lux(data.light);

        temperature_data.value = data.temperature;
        humidity_data.dials.dial[0].value = data.humidity;
        light_data.value = data.light;
        light_data.chart.gaugeRadius = data.light / 2;

        temperature_chart.setJSONData(temperature_data);
        humidity_chart.setJSONData(humidity_data);
        light_chart.setJSONData(light_data);

        var datetime = data.moment.split(" ");
        $("#elem-date").html(datetime[0]);
        $("#elem-time").html(datetime[1]);

        $("#elem-temperature-value").html(data.temperature+"°C");
        $("#elem-humidity-value").html(data.humidity+"%");
        $("#elem-light-value").html(data.light+"%");
    });

    $('.mydate').html("Avui");
    $('#panel-now').show();
    $('#button-today').hide();
    $.getJSON("/api/casa/dates/today", function(readings) {
        showReadings(readings);
    });
}


function clickToday() {
    date = null;
    $('.mydate').html("Avui");
    $('#panel-now').show();
    $('#button-today').hide();
    $.getJSON("/api/casa/dates/today", function(readings) {
        showReadings(readings);
    });
}



setInterval(UpdateReadings, 60*1000);  // update each minute
setInterval(UpdateDates, 60*60*1000);  // update each hour
