/*
=================================================================================================
Instituicao: FIAP
Arquivo    : Engine.cpp
Programa   : Comportamento e manipulacao dos componentes de um robo motorizado seguidor de linha
Autor(es)  : Alex Cantero, Bruno Espadaro, Floriano Cabral, Klinton Sae Lee e Nicholas M. Dantas
Turma      : 1ECR
Versao     : 2.0
=================================================================================================
*/

#include "Engine.h"

Engine::Engine(short directionPort = 0, short speedPort = 0, short defaultSpeed = 70)
{
	this->directionPort = directionPort;
	this->speedPort = speedPort;
	this->speed = defaultSpeed;
}

void Engine::Initialize()
{
	// Configuracao Motor
	// 2x2A - Ponto H L298P
	pinMode(speedPort, OUTPUT);
	pinMode(directionPort, OUTPUT);
}

void Engine::Start(Motion motion = Motion::Clockwise)
{
	analogWrite(speedPort, speed);
	digitalWrite(speedPort, motion);
	digitalWrite(directionPort, motion);

	this->currentMotion = motion;
}

void Engine::Stop(Motion motion = Motion::Anticlockwise)
{
	analogWrite(speedPort, speed);
	digitalWrite(speedPort, motion);
}

void Engine::Push()
{
	analogWrite(speedPort, speed * (SPEED_ADDITION_RATE + 1));
}

void Engine::SlowDown()
{
	analogWrite(speedPort, speed * SPEED_REDUCTION_RATE);
}

void Engine::Reverse()
{
	// Operacao 'XOR' e executada para inverter o movimento atual
	// Independente de qual seja, horario ou antihorario
	// 1 'XOR' 3 -> 2
	// 2 'XOR' 3 -> 1
	digitalWrite(directionPort, currentMotion ^ 3);
}