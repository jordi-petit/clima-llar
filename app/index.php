
<?


function get_readings($date) {
    $out = `grep $date /home/jpetit/llob158.txt`;
    return explode("\n", $out);
}


function parse_reading($line) {
    $elems = explode(" ", $line);
    $reads = explode(":", $elems[3]);
    return array(
        line        => $line,
        date        => $elems[0],
        time        => $elems[1],
        local_date  => substr($elems[0], 8, 2) . "/" . substr($elems[0], 5, 2) . "/" . substr($elems[0], 0, 4),
        short_time  => substr($elems[1], 0, 5),
        place       => $reads[0],
        temperature => $reads[1],
        humidity    => $reads[2],
        light       => round($reads[3] / 1024 * 100),
    );
}



function show_page() {
    $date = date("Y-m-d");
    $readings = get_readings($date);
    $last = $readings[sizeof($readings) - 2];

    $data = parse_reading($last);

    $c = 0;
    $temperature_list = "";
    $humidity_list = "";
    $light_list = "";
    $temperature_first = true;
    $humidity_first = true;
    $light_list_first = true;
    foreach ($readings as $reading) {
        $info = parse_reading($reading);

        $y = substr($info[date], 0, 4);
        $m = substr($info[date], 5, 2);
        $d = substr($info[date], 8, 2);
        $h = substr($info[time], 0, 2);
        $min = substr($info[time], 3, 2);
        $s = substr($info[time], 6, 2);
        $javadate = "Date.UTC($y, $m, $d, $h, $min, $s)";

        if ($info[temperature]) {
            $info[temperature] += rand(0,100)/100;
            if (!$temperature_first) $temperature_list .= ", ";
            else $temperature_first = false;
            $temperature_list .= "[$javadate, $info[temperature]]";
        }
        if ($info[humidity]) {
            if (!$humidity_first) $humidity_list .= ", ";
            else $humidity_first = false;
            $humidity_list .= "[$javadate, $info[humidity]]";
        }
        if ($info[light]) {
            if (!$light_first) $light_list .= ", ";
            else $light_first = false;
            $light_list .= "[$javadate, $info[light]]\n";
        }
        $c++;
    }

    print "

<!DOCTYPE html>
<html lang='en'>
    <head>

        <meta charset='utf-8'/>
        <meta http-equiv='X-UA-Compatible' content='IE=edge'/>
        <meta name='viewport' content='width=device-width, initial-scale=1'/>
        <meta http-equiv='Refresh' content='60'>

        <title>clima-llar-app</title>

        <link href='bower_components/bootstrap/dist/css/bootstrap.min.css' rel='stylesheet'/>
        <link rel='shortcut icon' href='favicon.ico'/>

    </head>
    <body>
        <div class='container'>
            <br/>

            <div class='jumbotron' style='padding-top: 6px; padding-bottom: 6px;'>
                <h1>Llobregat 158</h1>
                <p>Clima Llar App</p1>
            </div>

            <div class='panel panel-default'>
                <div class='panel-heading'>
                    <b>
                        TEST
                        <span class='pull-right'>$data[local_date]</span>
                    </b>
                </div>
                <div class='panel-body'>
                    <div>
                        <table id='readings' width='100%' border='0' cellpadding='0' cellspacing='0'>
                            <tr>
                                <td align='center'>
                                    <small>Temperatura</small>
                                </td>
                                <td align='center'>
                                    <small>Humitat</small>
                                </td>
                                <td align='center'>
                                    <small>Llum</small>
                                </td>
                            </tr>
                            <tr>
                                <td align='center'>
                                     <div id='chart-container2'></div>
                                </td>
                                <td align='center'>
                                    <div id='chart-container1'></div>
                                </td>
                                <td align='center'>
                                    <div id='chart-container3'></div>
                                </td>
                            </tr>
                        </table>
                    </div>
                </div>
            </div>



            <div class='panel panel-default'>
                <div class='panel-heading'>
                    <b>
                        $data[short_time]
                        <span class='pull-right'>$data[local_date]</span>
                    </b>
                </div>
                <div class='panel-body' style=''>
                    <div>
                        <table id='readings' width='100%' border='0' cellpadding='0' cellspacing='0'>
                            <tr>
                                <td align='center'>
                                    <small>Temperatura</small>
                                </td>
                                <td align='center'>
                                    <small>Humitat</small>
                                </td>
                                <td align='center'>
                                    <small>Llum</small>
                                </td>
                            </tr>
                            <tr>
                                <td align='center'>
                                    <div id='temperature-gauge' ></div>
                                </td>
                                <td align='center'>
                                    <div id='humidity-gauge' ></div>
                                </td>
                                <td align='center'>
                                    <div id='light-gauge' ></div>
                                </td>
                            </tr>
                        </table>
                    </div>
                </div>
            </div>

            <div class='panel panel-default'>
                <div class='panel-heading'>
                    <b>
                        Temperatura
                        <span class='pull-right'>$data[local_date]</span>
                    </b>
                </div>
                <div class='panel-body'>

                    <div class='flot-chart'>
                        <div class='flot-chart-content' id='temperature-chart' style='height: 175px;'></div>
                    </div>
                </div>
            </div>

            <div class='panel panel-default'>
                <div class='panel-heading'>
                    <b>
                        Humitat
                        <span class='pull-right'>$data[local_date]</span>
                    </b>
                </div>
                <div class='panel-body'>

                    <div class='flot-chart'>
                        <div class='flot-chart-content' id='humidity-chart' style='height: 175px;'></div>
                    </div>
                </div>
            </div>

            <div class='panel panel-default'>
                <div class='panel-heading'>
                    <b>
                        Llum
                        <span class='pull-right'>$data[local_date]</span>
                    </b>
                </div>
                <div class='panel-body'>

                    <div class='flot-chart'>
                        <div class='flot-chart-content' id='light-chart' style='height: 175px;'></div>
                    </div>
                </div>
            </div>

        </div>

        <script src='bower_components/jquery/dist/jquery.min.js'></script>
        <script src='bower_components/bootstrap/dist/js/bootstrap.min.js'></script>

        <script src='bower_components/flot/excanvas.min.js'></script>
        <script src='bower_components/flot/jquery.flot.js'></script>
        <script src='bower_components/flot/jquery.flot.pie.js'></script>
        <script src='bower_components/flot/jquery.flot.resize.js'></script>
        <script src='bower_components/flot/jquery.flot.time.js'></script>

        <script src='https://www.gstatic.com/charts/loader.js'></script>

        <script type='text/javascript' src='bower_components/fusioncharts/fusioncharts.js'></script>
        <script type='text/javascript' src='bower_components/fusioncharts/themes/fusioncharts.theme.fint.js'></script>

    </head>
        <script>


$(document).ready(function() {

    var gauge_size = $('#readings').width() / 3;
    if (gauge_size > 200) gauge_size = 200;
    console.log(gauge_size);

    google.charts.load('current', {
        'packages': ['gauge']
    });

    google.charts.setOnLoadCallback(drawChart1);
    google.charts.setOnLoadCallback(drawChart2);
    google.charts.setOnLoadCallback(drawChart3);

    function drawChart1() {

        var data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['°C', $data[temperature]]
        ]);

        var options = {
            width: gauge_size,
            height: gauge_size,
            greenFrom: 0,
            greenTo: 15,
            yellowFrom: 15,
            yellowTo: 25,
            redFrom: 25,
            redTo: 40,
            greenColor: 'LightSkyBlue',
            yellowColor: 'Bisque',
            redColor: 'LightCoral',
            min: 0,
            max: 40,
            minorTicks: 5
        };

        var chart = new google.visualization.Gauge(document.getElementById('temperature-gauge'));
        chart.draw(data, options);
    }


    function drawChart2() {

        var data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['%', $data[humidity]]
        ]);

        var options = {
            width: gauge_size,
            height: gauge_size,
            greenFrom: 0,
            greenTo: 100,
            greenColor: 'LightGray',
            min: 0,
            max: 100,
            minorTicks: 10
        };

        var chart = new google.visualization.Gauge(document.getElementById('humidity-gauge'));
        chart.draw(data, options);

    }

    function drawChart3() {

        var data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['', $data[light]]
        ]);

        var options = {
            width: gauge_size,
            height: gauge_size,
            greenFrom: 0,
            greenTo: 100,
            greenColor: 'LightGray',
            min: 0,
            max: 100,
            minorTicks: 10
        };

        var chart = new google.visualization.Gauge(document.getElementById('light-gauge'));
        chart.draw(data, options);

    }
});


$(document).ready(function() {
    var data = [{
        data: [
            $temperature_list
        ],
        //label: 'Temperature',
        color: 'green'
    }];
    var options = {
        legend: {
            position: 'se',
        },
        xaxis: {
            mode: 'time'
                //timezone: 'browser'
        }
    };

    $.plot($('#temperature-chart'), data, options);
});

$(document).ready(function() {
    var data = [{
        data: [
            $humidity_list
        ],
        //label: 'humidity',
        color: 'blue'
    }];
    var options = {
        legend: {
            position: 'se',
        },
        xaxis: {
            mode: 'time'
                //timezone: 'browser'
        }
    };

    $.plot($('#humidity-chart'), data, options);
});

$(document).ready(function() {
    var data = [{
        data: [
            $light_list
        ],
        //label: 'light',
        color: 'red'
    }];
    var options = {
        legend: {
            position: 'se',
        },
        xaxis: {
            mode: 'time'
                //timezone: 'browser'
        }
    };

    $.plot($('#light-chart'), data, options);
});



$(document).ready(function() {
    var gauge_size = $('#readings').width() / 3;
    var oR = 70;
    var iR = 65;
    var csatGauge = new FusionCharts({
        'type': 'angulargauge',
        'renderAt': 'chart-container1',
        'width': gauge_size,
        'height': gauge_size,
        'dataFormat': 'json',
        'dataSource': {
            'chart': {
                'numbersuffix': '%',
                'lowerLimit': '0',
                'upperLimit': '100',
                'gaugeFillMix': '{dark-40},{light-40},{dark-20}',
                'gaugeFillRatio': '85',
                'gaugeOuterRadius': oR,
                'gaugeInnerRadius': iR,
                'pivotRadius': '10',
                'showValue': '1',
                'majorTMNumber': '5',
                'minorTMNumber': '2',
                'gaugeStartAngle': '155',
                'gaugeEndAngle': '25',

                'theme': 'fint'
            },
            'colorRange': {
                'color': [{
                    'minValue': '0',
                    'maxValue': '33',
                    'code': '#FF8080'
                }, {
                    'minValue': '33',
                    'maxValue': '66',
                    'code': '#80FF80'
                }, {
                    'minValue': '66',
                    'maxValue': '100',
                    'code': '#8080FF'
                }]
            },
            'dials': {
                'dial': [{
                    'bgColor': '#666666,#FFFFFF,#666666',
                    'value': '$data[humidity]'
                }]
            }
        }
    });

    csatGauge.render();
});


// *************************************************



$(document).ready(function() {
    var gauge_size = $('#readings').width() / 3;
    var thermo = new FusionCharts({
        'type': 'thermometer',
        'renderAt': 'chart-container2',
        'width': gauge_size,
        'height': gauge_size,
        'dataFormat': 'json',
        'dataSource': {
            'chart': {
                'subcaptionFontBold': '0',
                'thmBulbRadius': '12',
                'lowerLimit': '-5',
                'upperLimit': '40',
                'numberSuffix': '°C',
                'bgColor': '#ffffff',
                'showBorder': '0',
                'thmFillColor': '#008ee4',
                'showTickMarks': '0',
                'showTickValues': '0'
            },
            'value': '$data[temperature]'
        }
    });

    thermo.render();
});



// *************************************************



$(document).ready(function() {
    var gauge_size = $('#readings').width() / 3;
    var radius = $data[light] * gauge_size/100.0 / 1.5;
    console.log('gauge_size = ', gauge_size);
    console.log('radius = ', radius);
    var bulb = new FusionCharts({
        'type': 'bulb',
        'renderAt': 'chart-container3',
        'width': gauge_size,
        'height': gauge_size,
        'dataFormat': 'json',
        'dataSource': {
            'chart': {
                'manageresize': '1',
                'bgcolor': 'ffffff',
                'decimals': '0',
                'numbersuffix': '%',
                'lowerlimit': '0',
                'upperlimit': '100',
                'showborder': '0',
                'is3D': '1',
                'placeValuesInside': '1',
                'gaugeRadius': radius
            },
            'colorrange': {
                'color': [{
                    'minvalue': '0',
                    'maxvalue': '10',
                    'code': '808080'
                }, {
                    'minvalue': '10',
                    'maxvalue': '100',
                    'code': 'F0F0A0'
                }, ]
            },
            'value': '$data[light]'

        }
    });

    bulb.render();
});

        </script>
    </body>
</html>

    ";
}



show_page();
