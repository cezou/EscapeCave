#define OLC_PGE_APPLICATION //Implémentation de la Base de Jeu
#include "GameBase.h"

#define OLC_PGEX_SOUND //Sons/Bruitages (bientot mis en places)
#include "GameBase_Sound.h"

#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include <condition_variable>
#include <ctime>
using namespace std;

static float timergauche = 0.0f;
static float timerdroite = 0.0f;
static float timersaut = 0.0f;
static float timerwait = 0.0f;
static float timerE = 0.0f;
static float timerCooldownE = 0.0f;
static float timerCooldownH = 0.0f;
static float timerLampe = 0.0f;
static float timercoin= 0.0f;


class EscapeCave : public olc::PixelGameEngine
{
public:
	EscapeCave()
	{
		sAppName = "Escape Cave";
	}

private:
	// Stockage du niveau
	wstring sLevel;
	int nLevelWidth;
	int nLevelHeight;

	// Propriétés d'apparition du joueur
	float fPlayerPosX = 2.0f;
	float fPlayerPosY = 0.0f;
	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;
	bool bPlayerOnGround = false;

	// Propriétés de la caméra
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;

	olc::vi2d vBobSize = { 32,32 };
	olc::vi2d vStoneSize = { 32,32 };
	std::unique_ptr<int[]> blocks;

	// Sprite Resources

	

	olc::Sprite* sprBob = nullptr;
//	olc::Decal* decBob = nullptr;
	olc::Sprite* sprBobJumping = nullptr;
//	olc::Decal* decBobJumping = nullptr;
	olc::Sprite* sprSky = nullptr;
//	olc::Decal* decSky = nullptr;
	olc::Sprite* sprStone = nullptr;
//	olc::Decal* decStone = nullptr;
	olc::Sprite* sprDino = nullptr;
	olc::Sprite* sprHintSign = nullptr;
//	olc::Decal* decDino = nullptr;
	olc::Sprite* sprChest= nullptr;
//	olc::Decal* decChest = nullptr;
	olc::Sprite* sprBandage = nullptr;
//	olc::Decal* decBandage = nullptr;
	olc::Sprite* sprPorte = nullptr;
	olc::Sprite* sprPorteOuverte = nullptr;
	olc::Sprite* sprSoon = nullptr;
	olc::Sprite* sprBalance = nullptr;
	olc::Sprite* sprTrapDoor = nullptr;
	olc::Sprite* sprTrapDoorOpen = nullptr;
	olc::Sprite* sprHiddenTrapDoor = nullptr;
	olc::Sprite* sprFakeStone = nullptr;
	olc::Sprite* sprIndice = nullptr;
	olc::Sprite* sprBalancePuzzle = nullptr;
	olc::Sprite* sprE = nullptr;
	olc::Sprite* sprPanneauDinoHUD = nullptr;
	olc::Sprite* sprPanneauHintHUD = nullptr;
	olc::Sprite* sprLampeOn = nullptr;
	olc::Sprite* sprLampeOff = nullptr;
	olc::Sprite* sprMenu = nullptr;
	olc::Sprite* sprPopup = nullptr;
	olc::Sprite* sprBoutonOui = nullptr;
	olc::Sprite* sprBoutonNon = nullptr;
	olc::Sprite* sprCoinHUD = nullptr;

//	olc::vecAudioSamples sampleCoffre = nullptr;

	// Selection des sprites
	int nDirModX = 0;
	int nDirModY = 0;
	int Temps = 0;
	int EtatCoffreX = 0;
	int EtatCoffreY = 0;
	int AvoirObjet = 0;
	int EtatBandageY = 0;
	int Avoirlaclef = 0;
	int Direction = 0;
	int Sauter = 0;
	float CourirVitesse = 0;
	float Animation = 0;
	int Bouger = 1;
	int Puzzle1 = 0;
	int AfficherE = 0;
	int ClignotageE = 0;
	int AfficherPanneau = 0;
	int AfficherPanneauDino = 0;
	int PanneauDejaAffiche = 0;
	int PanneauDejaAffiche2 = 0;
	int HintCoin = 0;
	int AnimationLampe = 0;
	float PositionTeteX = 0;
	float PositionTeteY = 0;
	int NumeroIndice = 0;
	int Popup = 0;
	int Menu = 0;
	int DemandeUtiliser = 0;
	int NombreUtilisation = 0;
	int WindowSelection = 0;
	int AfficherTextes = 0;
	int Niveau = 1;
	int AnimationHUD = 0;
	int AfficherHUD = 0;
//	int olc::vecAudioSamples()


	

public:
	bool OnUserCreate() override
	{
		nLevelWidth = 30;
		nLevelHeight = 34; 
		sLevel += L"##.###########################";
		sLevel += L"##.#1.......##################"; // 1 = (4;1)
		sLevel += L"##...........#################";
		sLevel += L"##....F......#################";
		sLevel += L"#............1################"; // 1 = (13;4) Prendre si PosX > 13 et < 14 et si PosY > 4 et < 5
		sLevel += L"#...H.........################"; // H = (4;5) D = (25,10)
		sLevel += L"######........################";
		sLevel += L"#######........###############";
		sLevel += L"######..........##############";
		sLevel += L"##C.............1#######..####"; // 1 = (16;9)
		sLevel += L"########.........######..D####"; // D = (25,10)
		sLevel += L"#########.........####...#####";
		sLevel += L"#########..........##...######";
		sLevel += L"#####..............#...#######";
		sLevel += L"#####.####............########";
		sLevel += L"#####-########################"; // - = (5;15)
		sLevel += L"#####.########################";
		sLevel += L"#####.########################";
		sLevel += L"#####.########################";
		sLevel += L"#####......###################"; // 19
		sLevel += L"#####.########################"; 
		sLevel += L"#####.########################";
		sLevel += L"#####.##1..#####1......#######"; // 1 = (8, 22) & (16, 22)
		sLevel += L"#####.####.#####.......#######";
		sLevel += L"#####.####.#####.......#######";
		sLevel += L"#####......L..........I#######";
		sLevel += L"##############################";
		sLevel += L"##############################";
		sLevel += L"##############################";
		sLevel += L"##############################";
		sLevel += L"############S..........#######";
		sLevel += L"##############################";
		sLevel += L"##############################";
		sLevel += L"##############################";

		
		sprStone = new olc::Sprite("./Sprite/Stone.png");
	//	decStone = new olc::Decal(sprStone);
		sprBob = new olc::Sprite("./Sprite/Sprite.png");
	//	decBob = new olc::Decal(sprBob);
		sprBobJumping = new olc::Sprite("./Sprite/Jumping.png");
	//	decBobJumping = new olc::Decal(sprBobJumping);
		sprSky = new olc::Sprite("./Sprite/Sky.png");
	//	decSky = new olc::Decal(sprSky);
		sprDino = new olc::Sprite("./Sprite/DinoSign.png");
	//	decDino= new olc::Decal(sprDino);
		sprChest = new olc::Sprite("./Sprite/Chest.png");
	//	decChest = new olc::Decal(sprChest);
		sprBandage = new olc::Sprite("./Sprite/Pensement.png");
	//	decBandage = new olc::Decal(sprBandage);
		sprPorte = new olc::Sprite("./Sprite/Door0.png");
		sprPorteOuverte = new olc::Sprite("./Sprite/Door1.png");
		sprSoon = new olc::Sprite("./Sprite/SoonSign.png");
		sprBalance = new olc::Sprite("./Sprite/Balance.png");
		sprHiddenTrapDoor = new olc::Sprite("./Sprite/HiddenTrapDoor.png");

		sprTrapDoor = new olc::Sprite("./Sprite/Trapdoor.png");
		sprTrapDoorOpen = new olc::Sprite("./Sprite/Trapdooropen.png");

		sprFakeStone = new olc::Sprite("./Sprite/FakeStone.png");
		sprIndice = new olc::Sprite("./Sprite/Indice.png");
		sprBalancePuzzle = new olc::Sprite("./Sprite/BALANCEPUZZLE.png");
		sprHintSign = new olc::Sprite("./Sprite/HintSign.png");
		sprE = new olc::Sprite("./Sprite/E-Key.png");
		sprPanneauHintHUD = new olc::Sprite("./Sprite/PanneauHintHUD.png");
		sprPanneauDinoHUD = new olc::Sprite("./Sprite/PanneauDinoHUD.png");
		sprLampeOn = new olc::Sprite("./Sprite/LampCoinSprite.png");
		sprLampeOff = new olc::Sprite("./Sprite/LampCoinEteinte.png");
		sprMenu = new olc::Sprite("./Sprite/Menu.png");
		sprPopup = new olc::Sprite("./Sprite/Popup.png");
		sprBoutonOui = new olc::Sprite("./Sprite/BoutonOui.png");
		sprBoutonNon = new olc::Sprite("./Sprite/BoutonNon.png");
		sprCoinHUD = new olc::Sprite("./Sprite/EscapeCoinHUD.png");
		return true;
	}


	virtual bool OnUserUpdate(float fElapsedTime)
	{

	/*	Animation = (fPlayerVelX / 800.0f);
		if (nDirModY > 6)
			nDirModY = 0;
		if (fPlayerVelX >= 8.0f)
		{
			CourirVitesse += Animation;

		}
		else
			CourirVitesse += Animation; 

		timer += fElapsedTime;
		if (timer > 1.0f) 
		{
			cout << "One second has passed";
			nDirModY += 1;
			timer = 0.0f;

		}*/


		// Utilités

		auto GetTile = [&](int x, int y)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				return sLevel[y * nLevelWidth + x];
			else
				return L' ';
		};

		auto SetTile = [&](int x, int y, wchar_t c)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				sLevel[y * nLevelWidth + x] = c;
		};

		PositionTeteX = fPlayerPosX + 0.5f;
		PositionTeteY = fPlayerPosY + 0.5f;

		//fPlayerVelX = 0.0f;
		//fPlayerVelY = 0.0f;

		// Déplacements par touche de clavier
		if (IsFocused())
		{
			// VOLER //
			//if (GetKey(olc::Key::Z).bHeld)
		//	{
			//	fPlayerVelY = -6.0f;
			
			//}

			//if ((GetKey(olc::Key::S).bHeld) && Bouger == 1)
			//{
				//fPlayerVelY = 6.0f;
		//	}

			if (((GetKey(olc::Key::Q).bHeld) || (GetKey(olc::Key::A).bHeld) || (GetKey(olc::Key::LEFT).bHeld)) && Bouger == 1)
			{
				fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) * fElapsedTime;
				nDirModX = 1 + AvoirObjet;

			}


			if (((GetKey(olc::Key::D).bHeld) || (GetKey(olc::Key::RIGHT).bHeld)) && Bouger == 1)
			{
				fPlayerVelX += (bPlayerOnGround ? 25.0f : 15.0f) * fElapsedTime;

				nDirModX = 0 + AvoirObjet;
			}


			if (((GetKey(olc::Key::SPACE).bHeld) || (GetKey(olc::Key::Z).bHeld) || (GetKey(olc::Key::UP).bHeld)) && Bouger == 1)
			{


				if ((fPlayerVelY == 0) && (bPlayerOnGround == true))
				{


					nDirModY = 8;
					fPlayerVelY = -12.0f;
				}
			}

			/*if (GetKey(olc::Key::B).bHeld)
			{
				if ((fPlayerVelY+fPlayerVelX) == 0)
				{
					nDirModY = nDirModY * int(fElapsedTime);
				}
				else
				{	
					nDirModY = 2;
					if ((int(fElapsedTime) % 2) == 0)
					{
						nDirModY = nDirModY + 1;
						if (nDirModY >= 4)
						{
							nDirModY = 2; 
						}

					}

				}
			}*/
		}


		

		// Gravité
		if (Bouger == 1)
		{
		fPlayerVelY += 20.0f * fElapsedTime;
		}
		// Drag
		if (bPlayerOnGround)
		{
			fPlayerVelX += -3.0f * fPlayerVelX * fElapsedTime;
			if (fabs(fPlayerVelX) < 0.01f)
				fPlayerVelX = 0.0f;
		}

		// Courir


		// Limiter la vitesse
		if (fPlayerVelX > 10.0f)
			fPlayerVelX = 10.0f;

		if (fPlayerVelX < -10.0f)
			fPlayerVelX = -10.0f;

		if (fPlayerVelY > 100.0f)
			fPlayerVelY = 100.0f;

		if (fPlayerVelY < -100.0f)
			fPlayerVelY = -100.0f;

		// Calcul de la potentielle nouvelle position
		float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
		float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

	/*	// Vérification de la récupération d'item (niveau 5)
		if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f) == L'o')
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'.');

		if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) == L'o')
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f, L'.');

		if (GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f) == L'o')
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'.');

		if (GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f) == L'o')
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f, L'.'); */

		// Vérification des colisions (avec l'acceptation de blocs comme "non solide")

		if (fPlayerVelX <= 0) // Aller a gauche
		{
			if (GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'.' 
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'P'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'H'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'F'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'K'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'I'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'T'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'O'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'1'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'0'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.0f) != L'/'



				|| GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'.'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'P'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'H'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'F'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'K'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'I'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'T'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'O'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'1'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'0'
				&& GetTile(fNewPlayerPosX + 0.2f, fPlayerPosY + 0.9f) != L'/')
			{
				fNewPlayerPosX = (int)fNewPlayerPosX + 0.8;
				fPlayerVelX = 0;
			}
		}
		else // Aller à droite
		{
			if (GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'.'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'F'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'H'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'K'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'P'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'I'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'T'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'O'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'1'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'0'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.0f) != L'/'

				|| GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'.'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'P'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'H'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'F'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'K'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'I'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'T'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'O'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'1'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'0'
				&& GetTile(fNewPlayerPosX + 0.8f, fPlayerPosY + 0.9f) != L'/')
			{
				fNewPlayerPosX = (int)fNewPlayerPosX + 0.2f;
				fPlayerVelX = 0;

			}
		}

		bPlayerOnGround = false;
		if (fPlayerVelY <= 0) // Monter
		{
			if (GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'.' 
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'P' 
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'H'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'F' 
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'K'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'I'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'T'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'O'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'1'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'0'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY) != L'/'


				|| GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'.' 
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'P' 
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'H'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'F' 
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'K'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'I'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'T'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'O'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'1'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'0'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY) != L'/')
			{
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				fPlayerVelY = 0;
			}
		}
		else // Descendre
		{
			if (GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'.' 
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'P' 
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'H'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'F' 
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'K' 
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'I'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'T'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'O'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'1'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'0'
				&& GetTile(fNewPlayerPosX + 0.2f, fNewPlayerPosY + 1.0f) != L'/'


				|| GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'.' 
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'P' 
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'H'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'F' 
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'K'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'I'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'T'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'O'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'1'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'0'
				&& GetTile(fNewPlayerPosX + 0.7f, fNewPlayerPosY + 1.0f) != L'/')
			{
				fNewPlayerPosY = (int)fNewPlayerPosY;
				fPlayerVelY = 0;
				bPlayerOnGround = true; // Le joueur a une surface solide sous ses pieds
			}

		}
		//timergauche = 0.0f;
		//timersaut = 0.0f;
	//	  timerdroite = 0.0f;
		//timerwait = 0.0f;

		if ((fPlayerVelX <= -0.2) && (bPlayerOnGround == true)) // Aller à gauche
		{
			//nDirModX = 1 + AvoirObjet;
			if ((nDirModY > 8) || (nDirModY < 2))
			{
				nDirModY = 2;
			}
			timergauche += fElapsedTime;
			if (timergauche > 0.1f)
			{
				nDirModY += 1;
				timergauche -= 0.1f;

			}
		}
		else if ((fPlayerVelX >= 0.2) && (bPlayerOnGround == true)) // Aller à droite
		{
			//nDirModX = 0 + AvoirObjet;
			if ((nDirModY > 8) || (nDirModY < 2))
			{
				nDirModY = 2;
			}
			timerdroite += fElapsedTime;
			if (timerdroite > 0.1f)
			{
				nDirModY += 1;
				timerdroite -= 0.1f;

			}
		}
		// Rester debout (attendre)
		else if ((fPlayerVelX < 0.2) && (fPlayerVelX > -0.2) && (bPlayerOnGround == true))
		{
			timerwait += fElapsedTime;
			if (timerwait > 1.0f)
			{
				nDirModY += 1;
				timerwait -= 1.0f;
				if (nDirModY > 1)
				{
				nDirModY = 0;
				}
			}
		}

		// Sauter
		
		else if ((fPlayerVelY < 0) && (bPlayerOnGround == false))
		{
			timersaut += fElapsedTime;
			if (timersaut > 0.4f)
			{
				nDirModY = 11;
				timersaut -= 0.4f;
			}
		}

		if (bPlayerOnGround == true)
		{
			timersaut = 0;
		}
		
		if (Bouger == 1)
		{
			timerCooldownE == 0;
			timerCooldownH == 0;
		}



		// Appliquer une nouvelle position du joueur
		if (Bouger==1)
		{ 
			fPlayerPosX = fNewPlayerPosX;
			fPlayerPosY = fNewPlayerPosY;
		}


		// Relier la caméra à la position du joueur
		fCameraPosX = fPlayerPosX;
		fCameraPosY = fPlayerPosY;


		// Dessiner le niveau
		SetPixelMode(olc::Pixel::MASK);
		int nTileWidth = 32;
		int nTileHeight = 32;
		int nVisibleTilesX = ScreenWidth() / nTileWidth;
		int nVisibleTilesY = ScreenHeight() / nTileHeight;


		// Calcul du bloc Haut gauche visible (x=0;y=0)
		float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
		float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f;


		// Serrer la camera sur les limites de la carte
		if (fOffsetX < 0) fOffsetX = 0;
		if (fOffsetY < 0) fOffsetY = 0;
		if (fOffsetX > nLevelWidth - nVisibleTilesX) fOffsetX = nLevelWidth - nVisibleTilesX;
		if (fOffsetY > nLevelHeight - nVisibleTilesY) fOffsetY = nLevelHeight - nVisibleTilesY;


		// 
		float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
		float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;


		// SALLE NUMERO 1


		// Ouverture du coffre
		if (((fPlayerPosX) - 2.0f <=1.0f) && ((fPlayerPosX) - 2.0f >= -1.0f) && ((fPlayerPosY)-10.0f <= 1.0f) && ((fPlayerPosY)-10.0f >= -1.0f) && (nDirModX == 1 + AvoirObjet) && EtatCoffreY== 0)
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				EtatCoffreY = 1;
				AvoirObjet = 2;
				nDirModX = 3;

				
			}
		}
		

		// Panneau Dinosaure // D = (25,10)
		
		//Pose du Bandage/*
		if (AvoirObjet == 2)
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				if (GetTile(PositionTeteX, PositionTeteY) == L'F')
				{
					SetTile(PositionTeteX, PositionTeteY, L'P');
					AvoirObjet = 4;
					nDirModX = 4;
					 
				}

					
					
			
			}

		}

		//Ouverture de la porte

		if (AvoirObjet == 4)
		{
			if ((fPlayerPosX >=4.0f) && (fPlayerPosX<= 6.0f) && (fPlayerPosY >= 14.0f) && (fPlayerPosY <= 16.0f) && (GetTile(5.0f, 15.0f) == L'-'))
			{
				if (GetKey(olc::Key::E).bPressed)
				{	
					SetTile(5.0f, 15.0f, L'/');
					AvoirObjet = 0;
					nDirModY = 0;
					nDirModX = 0;
					Niveau = 2;
					NombreUtilisation = 0;
				}
			}
		}
	

		// SALLE NUMERO 2
		
		if (fPlayerPosY - 18.0f >= 1.0f)
		{
			if (GetKey(olc::Key::G).bHeld)
			{
				SetTile(11.0f, 25.0f, L'K');
				Niveau = 3;
				NombreUtilisation = 0;
			}
		}

		//SALLE NUMERO 3

		if (GetKey(olc::Key::E).bPressed)
		{
			if (GetTile(fPlayerPosX, fPlayerPosY) == L'I')
			{
				SetTile(fPlayerPosX, (fPlayerPosY + 1), L'T');
				SetTile(fPlayerPosX, (fPlayerPosY + 2), L'O');
				SetTile(fPlayerPosX, (fPlayerPosY + 3), L'O');
				SetTile(fPlayerPosX, (fPlayerPosY + 4), L'O');
			}
		}
		//if (fPlayerPosY - 18.0f >= 1.0f) 



		// Draw visible tile map

		for (int x = -1; x < nVisibleTilesX + 1; x++)
		{
			for (int y = -1; y < nVisibleTilesY + 1; y++)
			{
				wchar_t sTileID = GetTile(x + fOffsetX, y + fOffsetY);
				switch (sTileID)
				{
					// // // DrawSprite(olc::vi2d(x, y) * vBlockSize, sprTile.get());
				case L'#': // Solid Block
					//FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::BLACK);
				//	DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprStone, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprStone, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					//DrawExplicitDecal(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprStone, 2 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
				case L'.': // Sky
//DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprSky.get(), (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight);
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::VERY_DARK_GREY);
					break;

				case L'P':
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprBandage, 0 * nTileWidth, EtatBandageY * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'D': // Panneau avec le dinosaure
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::VERY_DARK_GREY);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprDino, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'H': // Panneau Hint
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::VERY_DARK_GREY);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprHintSign, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'S': // Panneau "Soon"
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::VERY_DARK_GREY);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprSoon, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'C':
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::VERY_DARK_GREY);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprChest, EtatCoffreX * nTileWidth, EtatCoffreY * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'L':
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::VERY_DARK_GREY);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprPorte, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'K':
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::VERY_DARK_GREY);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprPorteOuverte, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'B':
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::VERY_DARK_GREY);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprBalance, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'I':					
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprIndice, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'T':					
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprHiddenTrapDoor, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
					
				case L'-':
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprTrapDoor, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'/':
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprTrapDoorOpen, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'O':					
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprFakeStone, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'1':
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprLampeOn, 0 * nTileWidth, AnimationLampe * nTileHeight, nTileWidth, nTileHeight);
					break;

				case L'0':
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, sprLampeOff, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
				}
			}
		}

		
		// JOUEUR
		DrawPartialSprite((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileWidth, sprBob, nDirModX * 32, nDirModY * 32, 32, 32);


	

		//Collectionner les Coin // 1 = (4;1) 2 = (13;4) 3 = (16;10) 
		
		if ((PositionTeteX >= 4.0f) && (PositionTeteX <= 5.0f) && (fPlayerPosY >= 1.0f) && (fPlayerPosY <= 2.0f) && ((GetTile(4.0f, 1.0f) == L'1')))
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				SetTile(4.0f, 1.0f, L'0');
				HintCoin += 1;
				Popup = 1;
				Bouger = 0;
				AfficherHUD = 1;
			}
		}

		
		// Notif Escape Coin

		if  (Popup == 1)
		{
			Bouger = 0;
			DrawPartialSprite(0, 0, sprPopup, 0 * 256, 0 * 240, 256, 240);
			DrawStringDecal({ 27,99 }, "Vous avez trouve un Escape Coin!", olc::Pixel(61,36,10), { 0.7f, 0.9f });
			DrawStringDecal({ 27,112 }, "Pour ouvrir le menu,", olc::Pixel(61, 36, 10), { 0.7f, 0.9f });
			DrawStringDecal({ 27,124 }, "Appuyez sur la touche H ;)", olc::Pixel(61, 36, 10), { 0.7f, 0.9f });
			timerCooldownE += fElapsedTime;
			if ((timerCooldownE > 1.0f) && ((GetKey(olc::Key::E).bPressed) || (GetKey(olc::Key::ESCAPE).bPressed)))
			{
				Popup = 0;
				Bouger = 1;
				timerCooldownE = 0;
			}

		}

		

		// Menu ECoin (Bouton H)

		if ((GetKey(olc::Key::H).bPressed) && (Bouger == 1))
		{
			Menu = 1;
			timerCooldownH = 0;
			AfficherTextes = 1;
		}


		// MENU ACTIVE
		if (Menu == 1)
		{
			Bouger = 0;
			DrawPartialSprite(0, 0, sprMenu, 0 * 256, WindowSelection * 240, 256, 240);
			DemandeUtiliser = 1;
			if (HintCoin == 0 && WindowSelection >= NombreUtilisation)
			{
				DrawStringDecal({ 27,99 }, "Vous n'avez pas d'Escape Coin.", olc::Pixel(61, 36, 10), { 0.7f, 0.9f });
			}

			else if (WindowSelection == NombreUtilisation)
			{
				DrawPartialSprite(59, 150, sprBoutonOui, 0 * 62, 0 * 29, 62, 29);
				DrawPartialSprite(136, 150, sprBoutonNon, 0 * 62, 0 * 29, 62, 29);
				DrawStringDecal({ 27,99 }, "Voulez-vous utiliser un", olc::Pixel(61, 36, 10), { 0.7f, 0.9f });
				DrawStringDecal({ 27,109 }, "Escape Coin?", olc::Pixel(61, 36, 10), { 0.7f, 0.9f });
				if ((GetMouseX() >= 59) && (GetMouseX() <= 121) && (GetMouseY() >= 150) && (GetMouseY() <= 179) && (GetMouse(0).bPressed) && (HintCoin > 0))
				{
					DemandeUtiliser = 1;
					HintCoin -= 1;
					NombreUtilisation += 1;

				}
				if ((GetMouseX() >= 136) && (GetMouseX() <= 198) && (GetMouseY() >= 150) && (GetMouseY() <= 179) && (GetMouse(0).bPressed))
				{
					Menu = 0;
				}
			}



			else if (WindowSelection > NombreUtilisation)
			{
				DrawStringDecal({ 27,99 }, "Vous devez d'abord deverouiller", olc::Pixel(61, 36, 10), { 0.7f, 0.9f });
				DrawStringDecal({ 26,109 }, "l'indice " + std::to_string(NombreUtilisation + 1) + "!", olc::Pixel(61, 36, 10), { 0.7f, 0.9f });
			}


			timerCooldownH += fElapsedTime;
			if ((GetMouseX() >= 10) && (GetMouseX() <= 69) && (GetMouseY() >= 61) && (GetMouseY() <= 84) && (GetMouse(0).bPressed))
			{
				WindowSelection = 0;

			}
			if ((GetMouseX() >= 71) && (GetMouseX() <= 130) && (GetMouseY() >= 61) && (GetMouseY() <= 84) && (GetMouse(0).bPressed))
			{
				WindowSelection = 1;
			}
			if ((GetMouseX() >= 132) && (GetMouseX() <= 191) && (GetMouseY() >= 61) && (GetMouseY() <= 84) && (GetMouse(0).bPressed))
			{
				WindowSelection = 2;
			}
			if ((GetMouseX() >= 199) && (GetMouseX() <= 249) && (GetMouseY() >= 61) && (GetMouseY() <= 84) && (GetMouse(0).bPressed))
			{
				WindowSelection = 3;
			}



			if ((WindowSelection == 0) && (NombreUtilisation >= 1))
			{
				if (Niveau == 1)
				{
					DrawStringDecal({ 27,99 }, "Il faut utiliser tous les elements", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
					DrawStringDecal({ 26,109 }, "de la salle. Rien n'est inutile.", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
				}

				if (Niveau == 2)
				{
					DrawStringDecal({ 27,99 }, "Il ne sert a rien d'essayer", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
					DrawStringDecal({ 26,109 }, "de remonter a gauche.", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
				}
				
				if (Niveau == 3)
				{
					DrawStringDecal({ 27,99 }, "Regardez partout dans la salle.", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
				}

			}

			if ((WindowSelection == 1) && (NombreUtilisation >= 2))
			{
				if (Niveau == 1)
				{
					DrawStringDecal({ 27,99 }, "A quoi correspond le Dinosaure?", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
					DrawStringDecal({ 26,109 }, "Regardez autour de vous.", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
				}

				if (Niveau == 2)
				{
					DrawStringDecal({ 23,99 }, "Souvenez vous de la maniere", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
					DrawStringDecal({ 22,109 }, "dont vous avez resolu la salle 1", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
					DrawStringDecal({ 23,119 }, "Mettez-vous au niveau du lampadaire.", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
				}
				if (Niveau == 3)
				{
					DrawStringDecal({ 27,99 }, "Un endroit est different des autres.", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });

				}
			}

			if ((WindowSelection == 2) && (NombreUtilisation >= 3))
			{
				if (Niveau == 1)
				{
					DrawStringDecal({ 27,99 }, "A quoi peut bien servir votre", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
					DrawStringDecal({ 26,109 }, "pensement?", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
				}

				if (Niveau == 2)
				{
					DrawStringDecal({ 27,99 }, "La forme de la grotte forme un 'G'", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });

				}

				if (Niveau == 3)
				{
					DrawStringDecal({ 27,99 }, "Regardez en bas a droite.", olc::Pixel(61, 36, 10), { 0.7f, 0.8f });
				}
			}

			if (WindowSelection == 3)
			{
				DrawStringDecal({ 27,99 }, "Repensez a tous les indices en ", olc::Pixel(193, 155, 100), { 0.7f, 0.8f });
				DrawStringDecal({ 26,109 }, "meme temps.", olc::Pixel(193, 155, 100), { 0.7f, 0.8f });
				DrawStringDecal({ 27,119 }, "Si vous n'arrivez toujours pas", olc::Pixel(193, 155, 100), { 0.7f, 0.8f });
				DrawStringDecal({ 26,129 }, "a trouver, rendez-vous sur le site", olc::Pixel(193, 155, 100), { 0.7f, 0.8f });
				DrawStringDecal({ 26,139 }, "web puis dans l'onglet Solution.", olc::Pixel(193, 155, 100), { 0.7f, 0.8f });
			}

			if ((timerCooldownH > 1.0f) && ((GetKey(olc::Key::H).bPressed) || (GetKey(olc::Key::ESCAPE).bPressed)))
			{
				Menu = 0;
				Bouger = 1;
				timerCooldownH = 0;
				AfficherTextes = 0;
				DemandeUtiliser = 0;
			}






		}
		// HUD Coin

		if (AfficherHUD == 1)
		{
			DrawPartialSprite(228, 0, sprCoinHUD, 0 * 24, AnimationHUD * 17, 24, 17);
			DrawStringDecal({ 231, 5 }, std::to_string(HintCoin), olc::Pixel(51, 29, 8), { 1.0f, 1.2f });
			DrawStringDecal({ 230, 4 }, std::to_string(HintCoin), olc::Pixel(255, 178, 46) , { 1.0f, 1.2f });
			
			if (HintCoin < 0)
			HintCoin = 0;
			/*timercoin += fElapsedTime;
			if (timercoin > 0.2f)
			{
				AnimationHUD += 1;
				timercoin -= 0.2f;
			}

			if (AnimationHUD > 7)
				AnimationHUD = 0; */
		} 




		if ((PositionTeteX >= 12.0f) && (PositionTeteX < 14.0f) && (fPlayerPosY > 3.0f) && (fPlayerPosY < 5.0f) && ((GetTile(13.0f, 4.0f) == L'1')))
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				SetTile(13.0f, 4.0f, L'0');
				HintCoin += 1;
			}
		}

		if ((PositionTeteX >= 16.0f) && (PositionTeteX < 17.0f) && (fPlayerPosY >= 9.0f) && (fPlayerPosY < 10.0f) && ((GetTile(16.0f, 9.0f) == L'1')))
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				SetTile(16.0f, 9.0f, L'0');
				HintCoin += 1;
			}
		}
		//(8, 22)
		if ((PositionTeteX >= 8.0f) && (PositionTeteX <= 9.0f) && (fPlayerPosY >= 22.0f) && (fPlayerPosY < 23.5f) && ((GetTile(8.0f, 22.0f) == L'1')))
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				SetTile(8.0f, 22.0f, L'0');
				HintCoin += 1;
			}
		}
		//(16, 22)
		if ((PositionTeteX >= 16.0f) && (PositionTeteX <= 17.0f) && (fPlayerPosY >= 22.0f) && (fPlayerPosY <= 23.0f) && ((GetTile(16.0f, 22.0f) == L'1')))
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				SetTile(16.0f, 22.0f, L'0');
				HintCoin += 1;
			}
		}

		
		// AFFICHAGE DU "E"       Bienvenue = (X = 4 ; Y = 5)      Dinosaure = (X = 25 ; Y = 10)
		if (((PositionTeteX >= 3.5f) && (PositionTeteX <= 5.5f) && (PositionTeteY >= 3.0f) && (PositionTeteY <= 8.0f) && (PanneauDejaAffiche == 0) && (Bouger == 1)) // Panneau "Bienvenue"

			|| ((PositionTeteX >= 24.5f) && (PositionTeteX <= 25.5f) && (PositionTeteY >= 9.5f) && (PositionTeteY <= 11.5f) && (PanneauDejaAffiche2 == 0) && (Bouger == 1))) // Panneau "Dinosaure"
		{
			AfficherE = 1;

		}
		else
			AfficherE = 0;

		// Clignotement DU "E"
 		if (AfficherE == 1)
		{
			DrawPartialSprite((122), (150), sprE, 0 * 32, ClignotageE * 32, 32, 32);
			timerE += fElapsedTime;
			if (timerE > 1.0f)
			{
				ClignotageE += 1;
				timerE -= 1.0f;
				if (ClignotageE > 1)
				{
					ClignotageE = 0;
				}
			}
		}


		// PANNEAU INDICE
		
		
		if ((PositionTeteX >= 3.5f) && (PositionTeteX <= 5.5f) && (PositionTeteY >= 3.0f) && (PositionTeteY <= 8.0f) && (Bouger == 1))
			
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				AfficherE = 0;
				PanneauDejaAffiche = 1;
				AfficherHUD = 1;
				AfficherPanneau = 1;

			}
		}
		
		if (AfficherPanneau == 1)
		{
			Bouger = 0;
			DrawPartialSprite(0, 0, sprPanneauHintHUD, 0 * 256, 0 * 240, 256, 240);
			DrawStringDecal({ 30,34 }, "Bienvenue dans Escape Cave.", olc::BLACK, { 0.7f, 0.9f });
			DrawStringDecal({ 30,50}, "Votre but est, comme le nom du jeu", olc::BLACK, { 0.7f, 0.9f });
			DrawStringDecal({ 29,60}, "l'indique, de vous echapper de la", olc::BLACK, { 0.7f, 0.9f });
			DrawStringDecal({ 29,70 }, "grotte. Pour ce faire, vous devrez", olc::BLACK, { 0.7f, 0.9f });
			DrawStringDecal({ 29,80 }, "resoudre l'enigme de chaque salle.", olc::BLACK, { 0.7f, 0.9f });
			DrawStringDecal({ 30,96 }, "Si vous etes perdu,", olc::BLACK, { 0.7f, 0.9f });
			DrawStringDecal({ 29,106 }, "vous pouvez a tout moment demander", olc::BLACK, { 0.7f, 0.9f });
			DrawStringDecal({ 29,116 }, "un indice a l'aide d'un Escape Coin.", olc::BLACK, { 0.7f, 0.9f });
			DrawStringDecal({ 30,126 }, "Regardez au-dessus de votre tete.", olc::BLACK, { 0.7f, 0.9f });
			DrawStringDecal({ 30,142 }, "Comme vous venez de constater,", olc::BLACK, { 0.7f, 0.9f });
			DrawStringDecal({ 29,152 }, "La touche 'utiliser' est E.", olc::BLACK, { 0.7f, 0.9f });

			timerCooldownE += fElapsedTime;
			if ((timerCooldownE > 1.0f) && ((GetKey(olc::Key::E).bPressed)||(GetKey(olc::Key::ESCAPE).bPressed)))
			{
				AfficherPanneau = 0;
				Bouger = 1;
				timerCooldownE = 0;
			}
		}




		// PANNEAU DINO

		if ((fPlayerPosX > 23.0f) && (fPlayerPosX < 26.0f) && (fPlayerPosY > 8.0f) && (fPlayerPosY <= 11.0f) && (Bouger == 1))
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				AfficherE = 0;
				PanneauDejaAffiche2 = 1;
				AfficherPanneauDino = 1;

			}
		}


		if (AfficherPanneauDino == 1)
		{
			Bouger = 0;
			DrawPartialSprite(0, 0, sprPanneauDinoHUD, 0 * 256, 0 * 240, 256, 240);

			timerCooldownE += fElapsedTime;
			if ((timerCooldownE > 1.0f) && ((GetKey(olc::Key::E).bPressed) || (GetKey(olc::Key::ESCAPE).bPressed)))
			{
				AfficherPanneauDino = 0;
				Bouger = 1;
				timerCooldownE = 0;
			}
		}



		// ESCAPE COIN

		timerLampe += fElapsedTime;
		if (timerLampe > 0.2f)
		{
			AnimationLampe += 1;
			timerLampe -= 0.2f;
		}

		if (AnimationLampe > 7)
			AnimationLampe = 0;


		// Compteur d'Escape Coin

		return true;
	}
};

int main()
{
	EscapeCave game;
	if (game.Construct(256, 240, 4, 4))
		game.Start();
	/*auto start = std::chrono::system_clock::now();
	while (true) 
	{
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		if (elapsed_seconds.count() > 1.0)
		{
			start = std::chrono::system_clock::now();
			UneSecondePassee = 1;

		}
		else
			UneSecondePassee = 0;
	} */
	return 0;
}
