#!/usr/bin/php
<?php

declare(strict_types=1);

define("COLOR_DEFAULT", "\033[0;0;0m");
define("COLOR_ERROR", "\033[31m");
define("COLOR_ALERT", "\033[31m");
define("COLOR_WARNING", "\033[33m");
define("COLOR_INFO", "\033[36m");
define("COLOR_SUCCESS", "\033[32m");

function hlight($msg, $color = COLOR_INFO)
{
    echo "$color$msg" . COLOR_DEFAULT . "\n";
}

function run($cmd, &$results = null, $silent = false, $throws = true, $descriptorspec = [
    ["pipe", "r"],  // stdin
    ["pipe", "w"],  // stdout
    ["pipe", "w"]   // stderr
]) {
    echo "$ $cmd\n";
    $results = ['output' => "", 'error' => "", 'return' => null];
    $process = proc_open($cmd, $descriptorspec, $pipes);
    if (is_resource($process)) {
        while ($s = fgets($pipes[1])) {
            $results['output'] .= $s;
            if (!$silent) print $s;
        }
        while ($s = fgets($pipes[2])) {
            $results['error'] .= $s;
            if (!$silent) print $s;
        }
        fclose($pipes[1]);
        $return = proc_close($process);
        $results['return'] .= $return;
    }
    if ($throws && $return) {
        throw new Exception("Command failed: $return", $return);
    }
    if ($return) {
        echo "return: $return\n";
    }
    return $return;
}

// Does not support flag GLOB_BRACE
function rglob($pattern, $flags = 0)
{
    $files = glob($pattern, $flags);
    foreach (glob(dirname($pattern).'/*', GLOB_ONLYDIR|GLOB_NOSORT) as $dir) {
        $files = array_merge([], ...[$files, rglob($dir . "/" . basename($pattern), $flags)]);
    }
    return $files;
}

function replace_extension($filename, $new_extension)
{
    $info = pathinfo($filename);
    return $info['dirname'] . '/' . $info['filename'] . '.' . $new_extension;
}

function build(
    $folder, $copts = "-std=c++17 -Wall -Wextra -Werror -Wpedantic -O3", $outext = "o", 
    $maincpp = "main.cpp", $mainexe = "main", $extras = "", $ofiles = [], $outdir = "build"
) {
    if (!$folder) {
        throw new Exception("No input folder", -1);
    }
    hlight("Build: $folder");

    hlight("Collecting subfolders and source files...");
    $dirs = rglob("$folder/*", GLOB_ONLYDIR);
    $hfiles = rglob("$folder/*.h");

    hlight("Createing build folder and subfolders ($folder)...");
    if (!is_dir($outdir)) {
        run("mkdir -p $outdir");
    }
    if (!is_dir("$outdir/$folder")) {
        run("mkdir -p $outdir/$folder");
    }
    foreach ($dirs as $dir) {
        if (!is_dir("$outdir/$dir")) {
            run("mkdir -p $outdir/$dir");
        }
    }

    hlight("Collecting and compile source files...");
    $ipaths = [];
    foreach ($hfiles as $hfile) {
        //echo "$hfile\n";
        $cfile = replace_extension($hfile, "cpp");
        if (!file_exists($cfile)) {
            // for header-only files
            $ipaths[] = dirname(realpath($hfile));
            continue;
        }
        $ofile = "$outdir/" . replace_extension($hfile, $outext);
        $ofiles[] = $ofile;

        // only make when modified
        if (file_exists($ofile)) {
            $otime = filemtime($ofile);
            if ($otime > filemtime($cfile) || $otime > filemtime($hfile)) {
                continue;
            }
        }
        
        run("g++ $copts -c $cfile -o $ofile $extras");
    }
    $ipaths = array_unique($ipaths);

    if ($maincpp) {
        $extras = 
            ($ipaths ? " -I" . implode(" -I", $ipaths) : "") .
            ($extras ? " $extras" : "");

        hlight("Compiling the main file...");
        run(
            "g++ $copts -c $folder/$maincpp -o $outdir/$mainexe.$outext"
            . $extras
        );

        hlight("Link objects to executable...");
        run("g++ $copts -o $outdir/$mainexe $outdir/$mainexe.$outext"
            . ($ofiles ? " " . implode(" ", $ofiles) : "")
            . $extras
        );
    }

    return $ofiles;
}


function clean($outdir)
{
    hlight("Clean...");
    run("rm -rf $outdir");
    run("rm -rf coverage");
    if (file_exists('coverage.info')) {
        run("rm -rf coverage.info");
    }
}

function builder($argv, $outdir = 'build', $copts = "-std=c++17 -Wall -Wextra -Werror -Wpedantic", $excludes = null, $extras = "")
{
    if (in_array("--help", $argv) || in_array("-h", $argv)) {
        hlight("Help:");
        echo "
  --help or -h: This help message
  --clean or -c: Do a cleanup before build
  --release or -r: Do a release build
  --debug or -d: Do a debug build
  --tests or -t: build tests, otherwise builds main executable
  --exec or -e: executes tests or main command after build
  --main or -m: set main/test .cpp filename. eg: --main program.cpp
";
        return;
    }

    $main_at = array_search("--main", $argv);
    if ($main_at === false) $main_at = array_search("-m", $argv);
    if ($main_at !== false) {
        $cppfile = $argv[$main_at + 1];
    }

    if (in_array("--clean", $argv) || in_array("-c", $argv)) {
        clean($outdir);
    }

    if (
        (!in_array("--clean", $argv) && !in_array("-c", $argv)) &&
        (!in_array("--release", $argv) && !in_array("-r", $argv)) &&
        (!in_array("--debug", $argv) && !in_array("-d", $argv))
    ) {
        echo "At least one should say debug or release or only cleanup? (use -d or -r or -c or --help for more infos)\n";
        return -1;
    }

    if (
        (in_array("--release", $argv) || in_array("-r", $argv)) &&
        (in_array("--debug", $argv) || in_array("-d", $argv))
    ) {
        echo "Exactly one should say debug or release? (use -d or -r or --help for more infos)\n";
        return -1;
    }

    if (
        (in_array("--release", $argv) || in_array("-r", $argv)) ||
        (in_array("--debug", $argv) || in_array("-d", $argv))
    ) {
        if (in_array("--release", $argv) || in_array("-r", $argv)) {
            $copts .= " -O3";
        }
        if (in_array("--debug", $argv) || in_array("-d", $argv)) {
            $copts .= " -O0 -g";
        }

        if ($copts) {
            $ofiles = build("src", $copts, "o", $cppfile ?? "main.cpp", "main", $extras);
            if (in_array("--tests", $argv) || in_array("-t", $argv)) {
                build("tests", $copts . " -fprofile-arcs -ftest-coverage", "o", $cppfile ?? "tests.cpp", "unittests", $extras, $ofiles);

                if (in_array("--exec", $argv) || in_array("-e", $argv)) {
                    hlight("Running unit tests:");
                    // run("node tests/mock_wssrv/index.mjs &");
                    run("$outdir/unittests");
                    hlight("Test passed", COLOR_SUCCESS);
                    hlight("Generating coverage info...");
                    run("lcov --no-external --directory . --capture --output-file coverage.info");
                    if (is_array($excludes)) {
                        $excludes = implode(' ', $excludes);
                    }
                    run("php lcov-fixer.php coverage.info $excludes");
                    run("genhtml -s --demangle-cpp -o coverage --dark-mode coverage.info");

                    run("lcov --summary coverage.info", $results, true);
                    preg_match('/lines......: (\d+\.\d+)%/', $results['output'], $matches);
                    $linesCoverage = $matches[1];
                    preg_match('/functions..: (\d+\.\d+)%/', $results['output'], $matches);
                    $functionsCoverage = $matches[1];

                    $coverError = false;
                    $coverMsg = "Code is fully covered:\n";
                    $coverColor = COLOR_SUCCESS;
                    if ($linesCoverage !== '100.0' && $functionsCoverage !== '100.0') {
                        $coverMsg = "Code is not fully covered:\n";
                        $coverColor = COLOR_WARNING;
                        $threshold = 95.0;
                        if ($linesCoverage < $threshold || $functionsCoverage < $threshold) {
                            $coverMsg = "Code coverage is below the threshold:\n";
                            $coverColor = COLOR_ALERT;
                            $coverError = true;                            
                        }
                    }
                    hlight($coverMsg . $results['output'], $coverColor);

                    hlight("For coverage info, run the following command:");
                    echo "google-chrome ./coverage/index.html\n";
                    if ($coverError) {
                       throw new Exception("Code coverage is below the acceptable threshold\n(lines: $linesCoverage%, functions: $functionsCoverage% < $threshold%)");
                    }
                }
            } else {
                if (in_array("--exec", $argv) || in_array("-e", $argv)) {
                    hlight("Running application:");
                    run("$outdir/main");
                }
            }
        }
    }
}


// release: $ php build.php -c -r
// debug: $ php build.php -c -d
// all test: (node node/mock_wssrv.mjs &) && php build.php -c -d -t -e

try {
    // first have to (re-)build libwebsocket (and any other dependencies):
    // $ yes | ./install.sh
    // (see more in the read.me file..)
    include "build.conf.php";
    return builder($argv, 'build', "-std=c++17 -Wall -Wextra -Werror -Wpedantic", 
        $excludes,
        implode(" ", $extras),
    );
} catch (Throwable $e) {
    $eCode = $e->getCode();
    hlight("Builder failed: (" . $eCode . ") " . $e->getMessage(), COLOR_ERROR);
    echo $e->getTraceAsString() . "\n";
    return $eCode ? $eCode : -1;
}
