/*
=================================================================================================
Instituicao: FIAP
Arquivo    : Engine.h
Programa   : Manipulacao de motor AC
Autor(es)  : Alex Cantero, Bruno Espadaro, Floriano Cabral, Klinton Sae Lee e Nicholas M. Dantas
Turma      : 1ECR
Versao     : 2.0
=================================================================================================
*/

#include <Arduino.h>

#ifndef Engine_h
#define Engine_h

// Definicoes de velocidade, portas, sensores, etc
#define ENGINE_VERSION				1.0  // Versao
#define SPEED_ADDITION_RATE			1.0f // Aumentara 100% da velocidade atual			
#define SPEED_REDUCTION_RATE		0.9f // Taxa de reducao do motor para ajustar a trajetoria. Reduzira 10%

// Enumerador de sentido de movimento
enum Motion { Stopped, Anticlockwise, Clockwise };

class Engine
{
public:
	// Construtores
	Engine(short directionPort = 0, short speedPort = 0, short defaultSpeed = 70);

	// Metodos
	void Initialize();
	void Start(Motion motion = Motion::Clockwise);
	void Stop(Motion motion = Motion::Anticlockwise);
	void Push();
	void SlowDown();
	void Reverse();
private:
	short directionPort;
	short speedPort;
	short speed;
	Motion currentMotion = Motion::Stopped;
};

#endif