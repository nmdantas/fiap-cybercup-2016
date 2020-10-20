/*
=================================================================================================
Instituicao: FIAP
Arquivo    : Skyline.cpp
Programa   : Comportamento e manipulacao dos componentes de um robo motorizado seguidor de linha
Autor(es)  : Alex Cantero, Bruno Espadaro, Floriano Cabral, Klinton Sae Lee e Nicholas M. Dantas
Turma      : 1ECR
Versao     : 2.0
=================================================================================================
*/

#include "Skyline.h"

int checkSensor(int sensorPort)
{
	int sum = 0;

	// Obtem a soma das leituras do sensor
	// 'n' vezes para garantir integridade a leitura
	for (int i = 0; i < N_TIMES; i++)
	{
		delay(100);

		sum += analogRead(sensorPort);
	}

	return sum / N_TIMES;
}

Skyline::Skyline()
{
	this->leftEngine = Engine(LEFT_ENGINE_SPEED_PORT, LEFT_ENGINE_DIRECTION_PORT);
	this->rightEngine = Engine(RIGHT_ENGINE_SPEED_PORT, RIGHT_ENGINE_DIRECTION_PORT);
}

void Skyline::Initialize()
{
	// Configuracao Bluetooth
	this->bluetoothSerial.begin(BLUETOOTH_PORT);

	// Configuracao Motor
	this->leftEngine.Initialize();
	this->rightEngine.Initialize();
	
	// Configuracao Servo
	this->leftServo.attach(LEFT_SERVO_PORT);
	this->rightServo.attach(RIGHT_SERVO_PORT);
}

void Skyline::ChoosePath()
{
	// Conexao Bluetooth
	String selectedPath = "";

	// Percorre toda a entrada
	while (this->bluetoothSerial.available())
	{
		selectedPath += (char)this->bluetoothSerial.read();
	}

	selectedPath.trim();

	// Caso nao consigo efetuar a leitura
	if (selectedPath.equals("") || selectedPath.length() != 2)
	{
		/*byPassedTurns[0] = 0;
		byPassedTurns[1] = 0;*/

		return;
	}

	// Verifica qual caminho deve seguir
	// e quantas curvas deve ignorar para chegar
	switch (selectedPath.charAt(0))
	{
	//case 49 /*1*/:
	//	byPassedTurns[0] = 0;

	//	break;
	//case 50 /*2*/:
	//	byPassedTurns[0] = 1;

	//	break;
	//case 51 /*3*/:
	//	byPassedTurns[0] = 2;

	//	break;
	//default:
	//	byPassedTurns[0] = 0;

	//	break;
	}

	// Verifica o caminho destino
	switch (selectedPath.charAt(1))
	{
	//case 65 /*A*/:
	//	byPassedTurns[1] = 3;

	//	break;
	//case 66 /*B*/:
	//	byPassedTurns[1] = 2;

	//	break;
	//case 67 /*C*/:
	//	byPassedTurns[1] = 0;

	//	break;
	//default:
	//	byPassedTurns[1] = 0;

	//	break;
	}

	this->PathInfo.SelectedPath = selectedPath;
	this->PathInfo.HasInfo = true;
}

void Skyline::UpdateState()
{
	int leftSensor = 0,
		rightSensor = 0,
		middleSensor = 0,
		leftTurnSensor = 0,
		rightTurnSensor = 0;

	leftSensor = checkSensor(LEFT_SENSOR_PORT);
	rightSensor = checkSensor(RIGHT_SENSOR_PORT);
	middleSensor = checkSensor(MIDDLE_SENSOR_PORT);
	leftTurnSensor = checkSensor(LEFT_TURN_SENSOR_PORT);
	rightTurnSensor = checkSensor(RIGHT_TURN_SENSOR_PORT);

	// Verifica se de ajustar a trajetoria para a esquerda
	if (leftSensor > middleSensor)
	{
		this->State.Adjust = Direction::Left;
	}
	else if (rightSensor > middleSensor)
	{
		this->State.Adjust = Direction::Right;
	}
	else
	{
		this->State.Adjust = Direction::None;
	}

	// Nao ha ajustes ha serem feitos no percuso e verifica se deve fazer a conversao para um dos lados
	if (this->State.Adjust == None && (leftTurnSensor != rightTurnSensor))
	{
		this->State.FlagCornerComplete = false;

		// Verifica para qual lado deve fazer a conversao
		if (leftTurnSensor > rightTurnSensor)
		{
			this->State.NextCorner = Direction::Left;
		}
		else
		{
			this->State.NextCorner = Direction::Right;
		}
	}
	else
	{
		this->State.FlagCornerComplete = true;
		this->State.NextCorner = Direction::None;
	}
}

void Skyline::Turn(Direction direction)
{
	// Se nao houver curva detectada previne a chamada
	if (this->State.NextCorner == None || this->State.FlagCornerComplete)
	{
		return;
	}

	switch (direction)
	{
	case Right:
		this->rightEngine.Stop();
		break;
	
	case Left:
		this->leftEngine.Stop();
		break;

	default:
		// Se o direcao nao for uma das duas esperadas aborta o procedimento
		return;
	}

	// Enquanto nao terminar a curva continua virando
	while (!this->State.FlagCornerComplete)
	{	
		delay(100);

		UpdateState();
	}

	// Se ainda nao realizou nenhuma curva 
	// Grava qual foi a primeira realizada
	// Isto servira para tomada de decisao
	// Nos casos de 'duvida'
	if (this->State.CornersMade == 0)
	{
		this->State.FirstCorner = direction;
	}

	this->State.CornersMade++;
	this->leftEngine.Start();
	this->rightEngine.Start();
}

void Skyline::Adjust()
{
	// Se nao houver ajuste detectado previne a chamada
	if (this->State.Adjust == None)
	{
		return;
	}

	switch (this->State.Adjust)
	{
	case Right:
		this->rightEngine.SlowDown();
		break;
	
	case Left:
		this->leftEngine.SlowDown();
		break;

	default:
		// Se o direcao nao for uma das duas esperadas aborta o procedimento
		return;
	}
}

void Skyline::AttachCargo()
{
	this->leftEngine.Stop();
	this->rightEngine.Stop();

	delay(250);

	// Aciona o servo
	for (int i = 0; i < 180; i++)
	{
		leftServo.write(i);
		rightServo.write(i);
	}

	this->State.FlagLoaded = true;
	this->leftEngine.Start();
	this->rightEngine.Start();
}

void Skyline::DetachCargo()
{
	int currentDegrees = leftServo.read();

	this->leftEngine.Stop();
	this->rightEngine.Stop();

	delay(250);

	// Aciona o servo
	for (int i = currentDegrees; i >= 0; i--)
	{
		leftServo.write(i);
		rightServo.write(i);
	}

	this->State.FlagLoaded = false;
	this->State.FlagEndOfLine = true;
}