/*
 =================================================================================================
 Instituicao: FIAP
 Programa   : Comportamento e manipulacao dos componentes de um robo motorizado seguidor de linha
 Autor(es)  : Alex Cantero, Bruno Espadaro, Floriano Cabral, Klinton Sae Lee e Nicholas M. Dantas
 Turma      : 1ECR
 Versao     : 1.0.5
 =================================================================================================
*/

#include "Skyline.h"

Skyline skyline;

void setup()
{
	skyline.Initialize();

	Serial.begin(9600);
}

void loop()
{
	// Verifica se o trajeto foi finalizado
	if (skyline.State.FlagEndOfLine)
	{
		return;
	}

	// Verifica se ja ha informacao sobre o caminho
	if (!skyline.PathInfo.HasInfo)
	{
		skyline.ChoosePath();
	}

	// Le os sensores e verifica os valores das flags para tomar uma
	// decisao em relacao ao caminho que devera ser tomado
	skyline.UpdateState();

	int cornersToIgnore = skyline.State.FlagLoaded ? 
						  skyline.PathInfo.CornersToIgnoreAfterCargo :
						  skyline.PathInfo.CornersToIgnoreBeforeCargo;
	
	Direction nextCorner = skyline.State.NextCorner;

	// Verifica se deve realizar alguma conversao
	// ou apenas ajustar a trajetoria
	// Tambem verifica se a conversao e devida em relacao ao caminho selecionado
	if (nextCorner != Direction::None && (skyline.State.CornersMade > cornersToIgnore))
	{
		// TODO: Um dos caminhos deve ser tratado de forma diferenciada.
		// 		 Existem dois ou mais casos que podem haver 'duvida' (doubt) 
		//       em relacao a qual curva devera ser contornada.

		// Verifica se e um caso de duvida, ha mais de uma curva encontrada
		if (skyline.State.FlagDoubt)
		{
			// Operacao 'XOR' e executada para selecionar 
			// a curva oposta a primeira realizada pelo robo
			// 1 'XOR' 3 -> 2 (Right)
			// 2 'XOR' 3 -> 1 (Left)
			nextCorner = (Direction)(skyline.State.FirstCorner ^ 3);
		}

		skyline.Turn(nextCorner);
	}
	else
	{
		// Verifica se deve ajustar a trajetoria
		skyline.Adjust();
	}

	// Verifica se ainda nao pegou a carga
	// e se deve acionar o servo-motor
	// TODO: Falta implementacao de quando esta flag sera verdadeira
	if (!skyline.State.FlagLoaded && skyline.State.FlagHasCargoToLoad)
	{
		skyline.AttachCargo();
	}

	// Verifica se a trajetoria terminou
	// E se deve descarregar a carga
	// TODO: Falta implementacao de quando esta flag sera verdadeira
	if (skyline.State.FlagReleaseCargo)
	{
		skyline.DetachCargo();
	}
}
