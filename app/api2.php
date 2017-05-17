<?

$key = "home";


function home_page() {
    global $key;

    $dates = glob("/home/jpetit/Projectes/clima-llar/server/home/*.txt");
    sort($dates);
    $last_date = $dates[sizeof($dates) - 1];

    $datas = [];

    $t = file_get_contents($last_date);
    $lines = explode("\n", $t);
    array_pop($lines);

    foreach ($lines as $line) {

        $inf1 = explode(" ", $line);
        $inf2 = explode(":", $inf1[3]);

        $data = array(
            datetime    => "$inf1[0] $inf1[1]",
            temperature => $inf2[1],
            humidity    => $inf2[2],
            light       => (int)($inf2[3] * 100 / 1024) * 1.7,        // 1.7 is a self adjusted factor
        );

        $datas [] = $data;
    }

    print(json_encode($datas));
}

home_page();
