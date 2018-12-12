<?php
$vetor = [90,10,20,25,30,40,50,57,42,1];
$tam = count($vetor) - 1;

echo "\nVETOR DESORDENADO\n";
for ($i = 0; $i <= $tam; $i++){
		echo $vetor[$i], " ";
}
echo "\n\n";

quick_sort($vetor, 0, $tam);


echo "VETOR ORDENADO\n";
for ($i = 0; $i <= $tam; $i++){
		echo $vetor[$i], " ";
}
echo "\n\n";

function quick_sort(&$v, $inicio, $fim){
	if ($fim > $inicio){
		$pivo = separa($v, $inicio, $fim);
		quick_sort($v, $inicio, $pivo-1); //ordena lado direito do pivo
		quick_sort($v, $pivo+1, $fim); //ordena lado esquerdo do pivo	
	}
}

function separa(&$vt, $inicio, $fim){
	$i = $inicio;
	$j = $fim;
	$pivo = $vt[$inicio] ;
	while($i < $j){
		while ($vt[$i] <= $pivo){
			$i++;
			if ($i >= $j)
				break;
		}
		while ($vt[$j] > $pivo){
			$j--;
		}
		if ($i < $j){
			$aux = $vt[$i];
			$vt[$i] = $vt[$j];
			$vt[$j] = $aux;

		}
	}
	$aux = $vt[$j];
	$vt[$j] = $pivo;
	$vt[$inicio] = $aux;

	return $j;
}



