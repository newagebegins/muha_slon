<?php

define('START_WORD', 'муха');
define('END_WORD', 'слон');
define('WORD_LEN', 4);

function main()
{
    mb_internal_encoding("UTF-8");

    $words = array();
    $adjacencyLists = array();
    $marked = array();
    $edgeTo = array();
    $bfsQueue = array();

    $dictionaryFile = fopen('dictionary.txt', 'r');

    if (!$dictionaryFile) {
        print "Couldn't open dictionary file!\n";
        return;
    }

    while (($word = fgets($dictionaryFile)) !== false) {
        $word = trim($word);
        if (mb_strlen($word) == WORD_LEN) {
            $words[] = $word;
        }
    }

    fclose($dictionaryFile);

    $wordsCount = count($words);

    for ($wordIndex1 = 0; $wordIndex1 < $wordsCount; ++$wordIndex1) {
        for ($wordIndex2 = 0; $wordIndex2 < $wordsCount; ++$wordIndex2) {
            if ($wordIndex1 == $wordIndex2) {
                continue;
            }

            $differentLettersCount = 0;
            for ($letterIdx = 0; $letterIdx < WORD_LEN; ++$letterIdx) {
                if (mb_substr($words[$wordIndex1], $letterIdx, 1) != mb_substr($words[$wordIndex2], $letterIdx, 1)) {
                    ++$differentLettersCount;
                    if ($differentLettersCount > 1) {
                        break;
                    }
                }
            }

            if ($differentLettersCount == 1) {
                $adjacencyLists[$wordIndex1][] = $wordIndex2;
            }
        }
    }

    $startWordIndex = array_search(START_WORD, $words);
    if (!$startWordIndex) {
        print "Couldn't find start word in the dictionary!\n";
        return;
    }

    $endWordIndex = array_search(END_WORD, $words);
    if (!$endWordIndex) {
        print "Couldn't find end word in the dictionary!\n";
        return;
    }

    $marked[$startWordIndex] = true;
    array_push($bfsQueue, $startWordIndex);

    while ($bfsQueue) {
        $wordIndex = array_shift($bfsQueue);
        foreach ($adjacencyLists[$wordIndex] as $adjacentIndex) {
            if (isset($marked[$adjacentIndex])) {
                continue;
            }

            $marked[$adjacentIndex] = true;
            $edgeTo[$adjacentIndex] = $wordIndex;
            array_push($bfsQueue, $adjacentIndex);
        }
    }

    if (!isset($marked[$endWordIndex])) {
        print "Can't convert start word to the end word.\n";
        return;
    }

    $path = array();
    $wordIndex = $endWordIndex;
    while (true) {
        $path[] = $words[$wordIndex];
        if ($wordIndex == $startWordIndex) {
            break;
        }
        $wordIndex = $edgeTo[$wordIndex];
    }

    while ($path) {
        printf("%s\n", array_pop($path));
    }
}

main();
