<?

$key = "home";


function home_page() {
    global $key;

    $dates = glob("/home/jpetit/Projectes/clima-llar/server/home/*.txt");
    sort($dates);
    $last_date = $dates[sizeof($dates) - 1];
    $last_line = trim(`tail -1 $last_date`);
    $inf1 = explode(" ", $last_line);
    $inf2 = explode(":", $inf1[3]);

    $data = array(
        date        => $inf1[0],
        time        => $inf1[1],
        temperature => $inf2[1],
        humidity    => $inf2[2],
        light       => (int)($inf2[3] * 100 / 1024) * 2,        // 2 is a self adjusted factor
    );
    print(json_encode($data));
}

home_page();
