using namespace std;

struct Point {
	int	x;
	int	y;
};

struct PaqueteBP {
	char	orden[100];
	pid_t 	pid = 0;
	Point	pos = (Point){-1,-1};
	char 	direccion = 0;
};

struct BolaPequena {
	pid_t			pid;
	Point			pos;
	char			direccion;
};

struct BolaRoja {
	int				id;
	Point			pos;
	int				radio;
	char			direccion;
};

struct BolaNegra {
	int				id;
	Point			pos;
	char			direccion;
};

#define PI 					3.14159265
#define SIGNEGRAVERDE		55
#define SIGTECLA 			54
#define SIGMOVAZUL	 		SIGUSR1
#define SIGTOGGLEAZUL		SIGUSR2
#define SIGLEERAZUL			52
#define SIGCOMPEQUENA		51
#define SIGDESTRUIRPEQUENA	50
#define SIGPRINCIPALPEQUENA	49

// Gráficos
#define	FRAMES_PER_SECOND	200
#define	PANTALLA_ANCHO		800
#define	PANTALLA_ALTO		600

// Teclado
#define KEYS_PER_SECOND		50

// Juego
#define JUEGO_FINAL			0
#define JUEGO_PAUSADO		1
#define JUEGO_PUNTOS		2


// Bola Verde
#define	BV_X				0
#define	BV_Y				sizeof(int)
#define BV_RADIO			30
#define PUNTOS_PERMANENCIA 	10

// Bola Pequeña
#define BP_X				0
#define BP_Y				sizeof(int)
#define BP_RADIO			14

// Bola Roja
#define TIEMPO_CRECIMIENTO	3
#define	BR_X				0
#define	BR_Y				sizeof(int)
#define	BR_RADIO			sizeof(int) * 2
#define BR_ACTIVA			sizeof(int) * 3
#define	BR_RADIO_INICIAL	30
#define	BR_RADIO_T2			40
#define	BR_RADIO_T3			50
#define	BR_RADIO_T4			60
#define	BR_DIR_IZQ_ARRIBA	0
#define	BR_DIR_IZQ_ABAJO	1
#define	BR_DIR_DCHA_ARRIBA	2
#define	BR_DIR_DCHA_ABAJO	3
#define BR_VELOCIDAD		3
#define BR_POS_1			0
#define BR_POS_2			80
#define BR_POS_3			170
#define BR_POS_4			280

// Bola Azul
#define BA_X				0
#define BA_Y				sizeof(int)
#define BA_ACTIVA			sizeof(int) * 2
#define BA_RADIO			30

//Bola Negra
#define BN_X				0
#define BN_Y				sizeof(int)
#define BN_ACTIVA			sizeof(int) * 2
#define BN_RADIO			30
#define	BN_DIR_IZQ_ARRIBA	0
#define	BN_DIR_IZQ_ABAJO	1
#define	BN_DIR_DCHA_ARRIBA	2
#define	BN_DIR_DCHA_ABAJO	3
#define BN_VELOCIDAD		1

// Parametros del archivo
int		ENERGIA_BOLA_VERDE,
		NUM_BOLAS_ROJAS,
		INTERVALO_ROJAS,
		INTERVALO_AZUL,
		NUM_BOLAS_NEGRAS,
		INTERVALO_NEGRAS,
		NUM_BOLAS_PEQUENAS,
		PUNTUACION_T1_ROJA,
		PUNTUACION_T2_ROJA,
		PUNTUACION_T3_ROJA,
		PUNTUACION_T4_ROJA,
		PUNTUACION_NEGRA,
		QUITA_ENERGIA_ROJA;

int 	pidPrincipal,
		pidBolaVerde,
		pidBolaRoja,
		pidBolaPequena,
		pidBolaAzul,
		pidBolaNegra;

long int	shmIdJuego;
int			*shmJuego;

long int	shmIdBolaVerde;
int			*shmBolaVerde;
char		tecla;
Point		posBV;
int 		pipeTeclaBV[2];

long int	shmIdBolaRoja;
int			*shmBolaRoja;
int 		shmTamanoBolaRoja;
SDL_TimerID *timerIdRoja;

BolaPequena bolasPequenas[5];
BolaPequena	bp;
int 		bpUltimoId;
int 		bpTotal;
long int	shmIdBolaPequena;
int			*shmBolaPequena;
int 		shmTamanoBolaPequena;

int 		pipeNegraVerde[2];
int 		pipePosBA[2];
int 		pipeIsVisibleBA[2];
Point		posAzul;
bool 		isVisibleAzul;

long int	shmIdBolaNegra;
int			*shmBolaNegra;
int 		shmTamanoBolaNegra;
SDL_TimerID *timerIdNegra;

SDL_Surface 	*pantalla,
				*srfInicio,
				*srfInstrucciones,
				*fondo,
				*srfFlash,
				*srfEnPausa,
				*srfHasPerdido,
				*srfBolaVerde,
				*srfBolaRoja,
				*srfBolaPequena,
				*srfBolaAzul,
				*srfBolaNegra,
				*srfPuntuacion,
				*srfVolumen;

TTF_Font		*fontPuntuacion;
SDL_Color 		colorBlanco,
				colorRojo;

Mix_Music 		*musica,
				*musicaInicio;

Mix_Chunk 		*shot,
				*explosion,
				*whip;

int main();
void procesoPrincipal();
void registerSigNegraVerde();
void sigNegraVerde(int);
void registerSigMovAzul();
void registerSigToggleAzul();
void registerSigLeerAzul();
void registerSigComPequena();
void registerSigDestruirPequena();
int crearProcesoBolaVerde();
void gestionarTeclaBolaVerde(int);
void dibujarBolaVerde(int, int);
int crearProcesoBolaPequena(int, char);
void sigComPequena(int);
void sigDestruirPequena(int, siginfo_t*, void*);
int obtenerNuevoIdPequena();
void registerSigPrincipalPequena();
void dibujarBolaPequena();
int crearProcesoBolaRoja();
int getIdLibreRoja();
void inicializarBolaRoja(int, BolaRoja*);
Uint32 moverBolaRoja(Uint32, void*);
void dibujarBolaRoja();
int crearProcesoBolaAzul();
void sigBA(int);
void sigToggleBA(int);
void sigLeerBA(int);
void dibujarBolaAzul();
int crearProcesoBolaNegra();
int getIdLibreNegra();
void inicializarBolaNegra(int, BolaNegra*);
Uint32 moverBolaNegra(Uint32, void*);
void dibujarBolaNegra();
void inicializar();
void cargarConfiguracion();
void mostrarPantallaInicio();
void dibujarFondo();
void dibujarFlash();
void dibujarEnPausa();
void dibujarHasPerdido();
void dibujarPuntuacion();
void dibujarVolumen();
int bolaRojaMasCercana();
bool detectarColisionBVBR();
double distanciaBolaVerdeBolaRoja(int);
void detectarColisionBPBR();
double distanciaBolaPequenaBolaRoja(int, int);
void detectarColisionBPBA();
int detectarColisionBNBR();
double distanciaBolaNegraBolaRoja(int, int);
void liberar();
int* crearVariosProcesos();