$pth = $args[0]
if(!$(test-path "$pth" -patht container)){write-host No correct path "$pth";exit}
dir "$pth\*.xml"|%{$a = ([xml](gc $_.fullname)).selectnodes('//package')
    for($i=0;$i -lt [int]($a.count);$i++){
            $a[$i].url
    }
}