/*
 =================================================================================================
 Instituicao: FIAP
 Arquivo    : Skyline.h
 Programa   : Comportamento e manipulacao dos componentes de um robo motorizado seguidor de linha
 Autor(es)  : Alex Cantero, Bruno Espadaro, Floriano Cabral, Klinton Sae Lee e Nicholas M. Dantas
 Turma      : 1ECR
 Versao     : 2.0
 =================================================================================================
*/

#include "Engine.h"
#include <Servo.h>
#include <SoftwareSerial.h>

#ifndef Skyline_h
#define Skyline_h

// Definicoes de velocidade, portas, sensores, etc
#define SKYLINE_VERSION				2.0  // Versao
#define N_TIMES						5    // Numero 'n' de vezes para obter a media dos leitores
#define BLUETOOTH_RX_PORT			10
#define BLUETOOTH_TX_PORT			11
#define BLUETOOTH_PORT				1991
#define LEFT_ENGINE_SPEED_PORT		5	 // Porta PWM
#define LEFT_ENGINE_DIRECTION_PORT  8
#define RIGHT_ENGINE_SPEED_PORT		6	 // Porta PWM
#define RIGHT_ENGINE_DIRECTION_PORT 9
#define LEFT_SERVO_PORT				12
#define RIGHT_SERVO_PORT			13
#define LEFT_SENSOR_PORT			0
#define RIGHT_SENSOR_PORT			1
#define MIDDLE_SENSOR_PORT			2
#define LEFT_TURN_SENSOR_PORT		3
#define RIGHT_TURN_SENSOR_PORT		4

// Enumeradores de sentido e direcao
enum Direction { None, Left, Right, Booth };

typedef struct State
{	
	int CornersMade = 0; // Esta propriedade sera atualizada no metodo 'Turn'
	
	bool FlagDoubt			= false;	// Flag para indicar duvida em relacao a qual curva deve seguir
	bool FlagLoaded			= false;	// Flag para indicar que ja possui carga
	bool FlagEndOfLine		= false;	// Flag para indicar fim do trajeto
	bool FlagCornerComplete = false;	// Flag para indicar curva concluida
	bool FlagHasCargoToLoad	= false;	// Flag para indicar que ha uma carga que deve ser carregada
	bool FlagReleaseCargo	= false;	// Flag para indicar que deve descarregar a carga

	Direction FirstCorner = Direction::None; // Esta propriedade sera atualizada no metodo 'Turn'
	Direction NextCorner = Direction::None;
	Direction Adjust = Direction::None;
};

typedef struct PathInfo
{
	String SelectedPath = "";
	int CornersToIgnoreBeforeCargo = 0;
	int CornersToIgnoreAfterCargo = 0;
	bool HasInfo = false;
};

class Skyline
{
public:
	// Construtores
	Skyline();
	
	// Propriedades
	State State;
	PathInfo PathInfo;

	// Metodos
	void Initialize();
	void ChoosePath();
	void UpdateState();
	void Turn(Direction direction);
	void Adjust();
	void AttachCargo();
	void DetachCargo();
private:
	Servo rightServo;
	Servo leftServo;
	Engine rightEngine;
	Engine leftEngine;
	SoftwareSerial bluetoothSerial = SoftwareSerial(BLUETOOTH_RX_PORT, BLUETOOTH_TX_PORT);
};

#endif