
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
    $gauge_size = 110;

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
                    <b>$data[local_date]
                    <span class='pull-right'>$data[short_time]</b></span>
                </div>
                <div class='panel-body'>
                    <table width='100%' style='margin-left: -16px;'>
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
                                <div id='temperature-gauge' style='width: 120; height: 120px;'></div>
                            </td>
                            <td align='center'>
                                <div id='humidity-gauge' style='width: 120; height: 120px;'></div>
                            </td>
                            <td align='center'>
                                <div id='light-gauge' style='width: 120; height: 120px;'></div>
                            </td>
                        </tr>
                    </table>
                    <center>
                    </center>
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

        <script>
            $(document).ready(function() {
               google.charts.load('current', {
                   'packages': ['gauge']
               });

               google.charts.setOnLoadCallback(drawChart1);
               google.charts.setOnLoadCallback(drawChart2);
               google.charts.setOnLoadCallback(drawChart3);

               function drawChart1() {

                   var data = google.visualization.arrayToDataTable([
                       ['Label', 'Value'],
                       ['℃', $data[temperature]]
                   ]);

                   var options = {
                       width: $gauge_size,
                       height: $gauge_size,
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
                       width: $gauge_size,
                       height: $gauge_size,
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
                       ['Light', $data[light]]
                   ]);

                   var options = {
                       width: $gauge_size,
                       height: $gauge_size,
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
        </script>
    </body>
</html>

    ";
}



show_page();
