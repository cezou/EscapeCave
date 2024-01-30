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
	float fPlayerPosY = 5.0f;
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
//	olc::Decal* decDino = nullptr;
	olc::Sprite* sprChest= nullptr;
//	olc::Decal* decChest = nullptr;
	olc::Sprite* sprBandage = nullptr;
//	olc::Decal* decBandage = nullptr;
	olc::Sprite* sprPorte = nullptr;
	olc::Sprite* sprPorteOuverte = nullptr;
	olc::Sprite* sprSoon = nullptr;

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


	

public:
	bool OnUserCreate() override
	{
		nLevelWidth = 30;
		nLevelHeight = 24; 
		sLevel += L"##############################";
		sLevel += L"####........##################";
		sLevel += L"###..........#################";
		sLevel += L"##....F......#################";
		sLevel += L"#.............################";
		sLevel += L"#.............################";
		sLevel += L"######........################";
		sLevel += L"#######........###############";
		sLevel += L"######..........##############";
		sLevel += L"##C..............#######..####";
		sLevel += L"########.........######..D####";
		sLevel += L"#########.........####...#####";
		sLevel += L"#########..........##...######";
		sLevel += L"#####.L............#...#######";
		sLevel += L"#####.###.............########";
		sLevel += L"#####.########################";
		sLevel += L"#####.########################";
		sLevel += L"#####.########################";
		sLevel += L"#####..........S##############";
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

		//fPlayerVelX = 0.0f;
		//fPlayerVelY = 0.0f;

		// Déplacements par touche de clavier
		if (IsFocused())
		{
			
			//if (GetKey(olc::Key::Z).bHeld)
		//	{
			//	fPlayerVelY = -6.0f;
			
			//}

			if (GetKey(olc::Key::S).bHeld)
			{
				fPlayerVelY = 6.0f;
			}

			if (GetKey(olc::Key::Q).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) * fElapsedTime;
				nDirModX = 1 + AvoirObjet;

			}
			if (GetKey(olc::Key::A).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) * fElapsedTime;
				nDirModX = 1 + AvoirObjet;
			}

			if (GetKey(olc::Key::D).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ? 25.0f : 15.0f) * fElapsedTime;
		
				nDirModX = 0 + AvoirObjet;
			}
			

			if (GetKey(olc::Key::SPACE).bHeld)
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
		fPlayerVelY += 20.0f * fElapsedTime;

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

	/*	// Vérification de la récupération d'item (niveau 3)
		if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f) == L'o')
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'.');

		if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) == L'o')
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f, L'.');

		if (GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f) == L'o')
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'.');

		if (GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f) == L'o')
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f, L'.'); */

		// Vérification des colisions
		if (fPlayerVelX <= 0) // Aller a gauche
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != L'.' && GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != L'P' && GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != L'F' && GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != L'K' || GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != L'.' && GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != L'P' && GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != L'F' && GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != L'K')
			{
				fNewPlayerPosX = (int)fNewPlayerPosX + 1;
				fPlayerVelX = 0;
			}
		}
		else // Aller à droite
		{
			if (GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != L'.' && GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != L'F' && GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != L'K' && GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != L'P'  || GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != L'.' && GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != L'P' && GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != L'F' && GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != L'K')
			{
				fNewPlayerPosX = (int)fNewPlayerPosX;
				fPlayerVelX = 0;

			}
		}

		bPlayerOnGround = false;
		if (fPlayerVelY <= 0) // Monter
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != L'.' && GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != L'P' && GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != L'F' && GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != L'K'  || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != L'.' && GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != L'P' && GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != L'F' && GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != L'K')
			{
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				fPlayerVelY = 0;
			}
		}
		else // Descendre
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) != L'.' && GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) != L'P' && GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) != L'F' && GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) != L'K' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != L'.' && GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != L'P' && GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != L'F' && GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != L'K')
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
		


		/*
		if (fPlayerVelX <= 0) // Moving Left
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != L'P' || GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != L'P')
			{
				fNewPlayerPosX = (int)fNewPlayerPosX + 1;
				fPlayerVelX = 0;
			}
		}
		else // Moving Right
		{
			if (GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != L'P' || GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != L'P')
			{
				fNewPlayerPosX = (int)fNewPlayerPosX;
				fPlayerVelX = 0;

			}
		}

		bPlayerOnGround = false;
		if (fPlayerVelY <= 0) // Moving Up
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != L'P' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != L'P')
			{
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				fPlayerVelY = 0;
			}
		}
		else // Moving Down
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) != L'P' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != L'P')
			{
				fNewPlayerPosY = (int)fNewPlayerPosY;
				fPlayerVelY = 0;
				bPlayerOnGround = true; // Player has a solid surface underfoot
				nDirModX = 1;

			}

		} */

		// Appliquer une nouvelle position du joueur
		fPlayerPosX = fNewPlayerPosX;
		fPlayerPosY = fNewPlayerPosY;

		// Relier la caméra à la position du joueur
		fCameraPosX = fPlayerPosX;
		fCameraPosY = fPlayerPosY;

		// Dessiner le niveau
		SetPixelMode(olc::Pixel::MASK);
		int nTileWidth = 32;
		int nTileHeight = 32;
		int nVisibleTilesX = ScreenWidth() / nTileWidth;
		int nVisibleTilesY = ScreenHeight() / nTileHeight;

		// Calcul du bloc Haut gauche visible
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

		//Pose du Bandage/*
		if (AvoirObjet == 2)
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				if (GetTile(fPlayerPosX, fPlayerPosY) == L'F')
				{
					SetTile(fPlayerPosX, fPlayerPosY, L'P');
					AvoirObjet = 4;
					nDirModX =4;
					 
				}

					
					
			
			}

		}

		//Ouverture de la porte
		if (AvoirObjet == 4)
		{
			if (((fPlayerPosX)-6.0f <= 1.0f) && ((fPlayerPosX)-6.0f >= -1.0f) && ((fPlayerPosY)-14.0f <= 1.0f) && ((fPlayerPosY)-14.0f >= -1.0f) && (nDirModX == 1 + AvoirObjet))
			{
				if (GetKey(olc::Key::E).bPressed)
				{
					SetTile(6.0f, 13.0f, L'K');
					AvoirObjet = 0;
					nDirModY = 0;
					nDirModX = 0;
				}
			}
		}
	



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
				case L'S': // Panneau avec le dinosaure
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
				}
			}
		}

		

		//OPENING CHEST
		//if 

		/*		//Pose du Bandage
		if (AvoirObjet == 3)
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				for (int x = -1; x < nVisibleTilesX + 1; x++)
				{
					for (int y = -1; y < nVisibleTilesY + 1; y++)
					{
						wchar_t sTileID = GetTile(x + fOffsetX, y + fOffsetY);
						switch (sTileID)
						{
						case L'.':
							DrawPartialSprite(x * int(fPlayerPosX), y * int(fPlayerPosY), sprBandage, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
							break;
						}
					}
				}
			}
		}
		*/
		// Draw Player
		//Fill((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileWidth, (fPlayerPosX - fOffsetX + 1.0f) * nTileWidth, (fPlayerPosY - fOffsetY + 1.0f) * nTileHeight, PIXEL_SOLID, FG_GREEN);
		DrawPartialSprite((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileWidth, sprBob, nDirModX * 32, nDirModY * 32, 32, 32);
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
