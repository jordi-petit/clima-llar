<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>clima-llar-app</title>
        <!-- Bootstrap -->
        <link href="bower_components/bootstrap/dist/css/bootstrap.min.css" rel="stylesheet">
        <!-- Favicon -->
        <link rel="shortcut icon"  href="favicon.ico" />
    </head>
    <body>
        <div class="container">
            <br/>

            <div class="jumbotron" style='padding-top: 6px; padding-bottom: 6px;'>
                <h1>Llobregat 158</h1>
                <p>Clima Llar App</p1>
            </div>

            <div class='panel panel-default'>
                <div class='panel-heading'>
                    Readings
                </div>
                <div class='panel-body'>
                    <table>
                        <tr>
                            <td>
                                <div id='temperature-gauge' style='width: 120; height: 120px;'></div>
                            </td>
                            <td>
                                <div id='humidity-gauge' style='width: 120; height: 120px;'></div>
                            </td>
                            <td>
                                <div id='light-gauge' style='width: 120; height: 120px;'></div>
                            </td>
                        </tr>
                    </table>
                </div>
            </div>

            <div class='panel panel-default'>
                <div class='panel-heading'>
                    Temperature
                </div>
                <div class='panel-body'>

                    <div class='flot-chart'>
                        <div class='flot-chart-content' id='temperature-chart' style='height: 175px;'></div>
                    </div>
                </div>
            </div>

            <div class='panel panel-default'>
                <div class='panel-heading'>
                    Humidity
                </div>
                <div class='panel-body'>

                    <div class='flot-chart'>
                        <div class='flot-chart-content' id='humidity-chart' style='height: 175px;'></div>
                    </div>
                </div>
            </div>

            <div class='panel panel-default'>
                <div class='panel-heading'>
                    Light
                </div>
                <div class='panel-body'>

                    <div class='flot-chart'>
                        <div class='flot-chart-content' id='light-chart' style='height: 175px;'></div>
                    </div>
                </div>
            </div>

        </div>
        <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
        <script src="bower_components/jquery/dist/jquery.min.js"></script>
        <!-- Include all compiled plugins (below), or include individual files as needed -->
        <script src="bower_components/bootstrap/dist/js/bootstrap.min.js"></script>


                <!-- Flot Charts JavaScript -->
                <script src='bower_components/flot/excanvas.min.js'></script>
                <script src='bower_components/flot/jquery.flot.js'></script>
                <script src='bower_components/flot/jquery.flot.pie.js'></script>
                <script src='bower_components/flot/jquery.flot.resize.js'></script>
                <script src='bower_components/flot/jquery.flot.time.js'></script>



                <!-- Google Charts JavaScript -->
                <script src='https://www.gstatic.com/charts/loader.js'></script>


                <script>


       $(document).ready(function(){
            var data = [
                {
                    data: [
                        [1, 20],
                        [2, 20],
                        [3, 22],
                        [4, 20],
                        [5, 19],
                        [6, 20],
                        [7, 22],
                        [8, 20],
                    ],
                    //label: 'Temperature',
                    color: 'green'
                }
            ];
            var options = {
                legend: {
                    position: 'se',
                }
            };

            $.plot($('#temperature-chart'), data, options);
        });

       $(document).ready(function(){
            var data = [
                {
                    data: [
                        [1, 20],
                        [2, 20],
                        [3, 22],
                        [4, 20],
                        [5, 19],
                        [6, 20],
                        [7, 22],
                        [8, 20],
                    ],
                    //label: 'Humidity',
                    color: 'red'
                }
            ];
            var options = {
                legend: {
                    position: 'se',
                }
            };

            $.plot($('#humidity-chart'), data, options);
        });

       $(document).ready(function(){
            var data = [
                {
                    data: [
                        [1, 20],
                        [2, 20],
                        [3, 22],
                        [4, 20],
                        [5, 19],
                        [6, 20],
                        [7, 22],
                        [8, 20],
                    ],
                    //label: 'Light',
                    color: 'blue'
                }
            ];
            var options = {
                legend: {
                    position: 'se',
                }
            };

            $.plot($('#light-chart'), data, options);
        });




               $(document).ready(function(){
                  google.charts.load('current', {'packages':['gauge']});

                  google.charts.setOnLoadCallback(drawChart1);
                  google.charts.setOnLoadCallback(drawChart2);
                  google.charts.setOnLoadCallback(drawChart3);

                  function drawChart1() {

                    var data = google.visualization.arrayToDataTable([
                      ['Label', 'Value'],
                      ['ccn', 3]
                    ]);

                    var options = {
                      width: 115, height: 115,
                      greenFrom: 0, greenTo: 9, yellowFrom: 9, yellowTo: 14, redFrom: 14, redTo: 20,
                      min: 0, max: 18,
                      minorTicks: 5
                    };

                    var chart = new google.visualization.Gauge(document.getElementById('temperature-gauge'));
                    chart.draw(data, options);
                  }


                  function drawChart2() {

                    var data = google.visualization.arrayToDataTable([
                      ['Label', 'Value'],
                      ['ccn ratio', 1.0]
                    ]);

                    var options = {
                      width: 115, height: 115,
                      greenFrom: 0, greenTo: 1.5, yellowFrom: 1.5, yellowTo: 2.25, redFrom: 2.25, redTo: 3,
                      min: 0, max: 3,
                      minorTicks: 5
                    };

                    var chart = new google.visualization.Gauge(document.getElementById('humidity-gauge'));
                    chart.draw(data, options);

                  }

                  function drawChart3() {

                    var data = google.visualization.arrayToDataTable([
                      ['Label', 'Value'],
                      ['dif ratio', 1.1]
                    ]);

                    var options = {
                      width: 115, height: 115,
                      greenFrom: 0, greenTo: 1.5, yellowFrom: 1.5, yellowTo: 2.25, redFrom: 2.25, redTo: 3,
                      min: 0, max: 3,
                      minorTicks: 5
                    };

                    var chart = new google.visualization.Gauge(document.getElementById('light-gauge'));
                    chart.draw(data, options);

                  }
            });

                </script>


    </body>
</html>
