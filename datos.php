<?php
	$base_de_datos = $_GET['datos'];
	if (empty($base_de_datos)){
		$base_de_datos = 'https://dl.dropboxusercontent.com/u/119376/ShiftCal.db';
	}
	unlink('./ShiftCal.db');
	//echo 'Datos desde '.$base_de_datos . "<br>";
	copy($base_de_datos,'ShiftCal.db');
	
	$calendarios = 0;
	$cadena = '';
    $db = new SQLite3('ShiftCal.db');
    $tablesquery = $db->query("SELECT * FROM calendar,shifts WHERE calendar.cal_shift_id1 = shifts.shift_rowid ORDER BY calendar.cal_date");
	$ano = '';
	$mes = '';
    while ($table = $tablesquery->fetchArray(SQLITE3_ASSOC)) {
        $este_mes = substr($table['cal_date'],4,2);
		$este_ano = substr($table['cal_date'],0,4);
		$este_dia = substr($table['cal_date'],6,2);
		$este_turno = $table['cal_shift_id1'];
		if ($ano == $este_ano) {
			if ($mes == $este_mes) {
				$cadena = $cadena . $este_turno;
			}
			else
			{
				$mes = $este_mes;
				$calendarios++;
				$cadena = $cadena . '","' . cal_days_in_month(CAL_GREGORIAN, $este_mes, $este_ano) . $ano . $mes . $este_turno;
			}
		}
		else
		{
			$ano = $este_ano;
			$mes = $este_mes;
			$calendarios++;
			$cadena = $cadena . '","' . cal_days_in_month(CAL_GREGORIAN, $este_mes, $este_ano) . $ano . $mes . $este_turno;
		}
	}
	$cadena = substr($cadena, 3) . '"]';
	$cadena = '["' . $cadena;
	//echo $cadena;
	$maximo = 14;
	$matriz = array();	
	$matriz = json_decode($cadena, TRUE);
	$inicio_matriz = count($matriz)-$maximo;
    if ($inicio_matriz < 0)
	{
		$inicio_matriz = 0;
	}
	echo '{"main":{'."<br>";
	$contador = 0;	
	$coma = ',';
	for ($i = $inicio_matriz;$i < count($matriz); $i++)
	{
		if ($i == (count($matriz)-1))
		{
			$coma = '';
		}
		echo '"data'.$contador. '":"'.$matriz[$i].'"'.$coma."<br>";
		$contador++;
	}
	echo '}}'
	
?>
