#include <iostream>
#include <Windows.h>
#include <chrono>
using namespace std;

// Constants
const short nScreenWidth = 128 * 2, nScreenHeight = 36 * 2;
const wchar_t Palette[] = L" .,-~*:;!=#$@"; // 13 shades
const int ColorPalette[] = { 0b0000, 0b0111, 0b0100, 0b0010, 0b0001, 0b0110 };
const float PI = 3.14159265359f;
const float fShadeFactor = 1.35f;

// Player variables
float fPlayerHeight = 0.6f, fPlayerX = 2.0f, fPlayerY = 2.0f, fPlayerZ = fPlayerHeight, fPlayerA = 2 * PI, fFOV = (PI / 180) * 90, fPlayerSpeed = 1.0f, fPlayerRotation = 1.0f, fPlayerRadius = 0.1f, fPlayerSprint = 1.0f;
bool bPlayerSprint = false;
const float fProjDist = (nScreenWidth / 2) / tanf(fFOV / 2);

// Map variables
short nMapWidth = 16, nMapHeight = 16;
short Map[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

const short nTextureWidth = 16, nTextureHeight = 16;
short Texture[][nTextureWidth * nTextureHeight] = {
	{ // Brick Texture
		11, 12, 12, 03, 11, 12, 11, 12,	12, 12, 12, 03, 11, 12, 12, 12,
		10, 11, 11, 03, 11, 11, 10, 11,	11, 11, 11, 03, 10, 11, 11, 11,
		10, 10, 11, 03, 10, 10, 11, 11,	10, 10, 11, 03, 11, 10, 11, 11,
		03, 03, 03, 03, 03, 03, 03, 03,	03, 03, 03, 03, 03, 03, 03, 03,
		12, 12, 11, 11, 12, 11, 11, 03,	11, 12, 12, 11, 12, 12, 11, 03,
		11, 10, 11, 11, 11, 11, 11, 03,	10, 11, 11, 11, 11, 11, 11, 03,
		11, 10, 10, 11, 10, 11, 11, 03,	11, 10, 11, 11, 10, 10, 10, 03,
		03, 03, 03, 03, 03, 03, 03, 03,	03, 03, 03, 03, 03, 03, 03, 03,
		11, 12, 12, 03, 11, 12, 11, 12,	12, 12, 12, 03, 11, 12, 12, 12,
		10, 11, 11, 03, 11, 11, 10, 11,	11, 11, 11, 03, 10, 11, 11, 11,
		10, 10, 11, 03, 10, 10, 11, 11,	10, 10, 11, 03, 11, 10, 11, 11,
		03, 03, 03, 03, 03, 03, 03, 03,	03, 03, 03, 03, 03, 03, 03, 03,
		12, 12, 11, 11, 12, 11, 11, 03,	11, 12, 12, 11, 12, 12, 11, 03,
		11, 10, 11, 11, 11, 11, 11, 03,	10, 11, 11, 11, 11, 11, 11, 03,
		11, 10, 10, 11, 10, 11, 11, 03,	11, 10, 11, 11, 10, 10, 10, 03,
		03, 03, 03, 03, 03, 03, 03, 03,	03, 03, 03, 03, 03, 03, 03, 03,
	},
	{ // Door Texture
		2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		2,  9, 10, 11,  4, 11, 10,  9,  4,  9,  9, 10,  4, 10, 11,  2,
		2,  9,  9, 10,  4, 10,  9,  9,  4,  9,  9,  9,  4,  9, 10,  2,
		2,  9,  9,  9,  4,  9,  9, 10,  4,  9,  9,  9,  4,  9,  9,  2,
		2,  9, 10,  9,  4,  9,  9,  9,  4, 10,  9,  9,  4,  9,  9,  2,
		2,  9,  9,  9,  4,  9,  9,  9,  4,  9,  9,  9,  4, 10,  9,  2,
		2,  9,  9,  9,  4,  9,  9,  9,  4,  9,  9,  9,  4,  9,  9,  2,
		2,  9,  9,  9,  4,  9,  9,  9,  4,  9,  9,  9,  4,  9,  9, 12,
		2,  9,  9,  9,  4,  9,  9,  9,  4,  9,  9,  9,  4,  9, 12, 11,
		2,  9,  8,  9,  4,  9,  9,  9,  4,  9,  9,  9,  4,  9,  7, 11,
		2,  9,  9,  9,  4,  8,  9,  9,  4,  8,  9,  9,  4,  9,  9,  1,
		2,  9,  9,  8,  4,  8,  9,  9,  4,  9,  9,  9,  4,  9,  9,  2,
		2,  9,  9,  8,  4,  9,  9,  8,  4,  9,  8,  8,  4,  8,  9,  2,
		2,  8,  9,  9,  4,  9,  8,  7,  4,  9,  8,  9,  4,  9,  8,  2,
		2,  7,  8,  9,  4,  9,  8,  8,  4,  8,  7,  8,  4,  8,  7,  2,
		2,  7,  8,  7,  4,  8,  7,  7,  4,  7,  7,  7,  4,  7,  7,  2,
	},
	{ // Old Door Texture
		2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		2, 10, 11,  2, 11, 10, 11,  2,  4, 10, 10, 11,  2, 11, 10,  2,
		2,  9, 10,  2, 10,  9, 10,  2,  4,  9,  9, 10,  2, 10,  9,  2,
		2, 10,  9,  2,  9,  9,  9,  2,  4, 10,  9,  9,  2, 10,  9,  2,
		2,  9,  9,  2, 10,  9,  9,  2,  4, 10,  9,  9,  2,  9, 10,  2,
		2,  9, 10,  2,  9, 10,  9,  2,  4,  9,  9, 10,  2,  9,  9,  2,
		2,  9,  9,  2,  9,  9,  9,  2,  4,  9,  9,  9,  2,  9,  9,  2,
		2,  9,  9,  2,  9,  9,  9,  2,  4,  9,  9,  9,  2,  9,  9,  2,
		2,  9,  9,  2,  9,  9, 12,  2,  4, 12,  9,  8,  2,  8,  9,  2,
		2,  9,  8,  2,  9,  9,  8,  2,  4,  8,  9,  9,  2,  8,  9,  2,
		2,  9,  9,  2,  8,  9,  9,  2,  4,  9,  9,  9,  2,  9,  8,  2,
		2,  8,  9,  2,  9,  8,  9,  2,  4,  9,  9,  8,  2,  9,  9,  2,
		2,  8,  9,  2,  9,  9,  8,  2,  4,  8,  9,  8,  2,  9,  8,  2,
		2,  7,  8,  2,  8,  9,  7,  2,  4,  9,  8,  7,  2,  7,  9,  2,
		2,  8,  7,  2,  7,  8,  7,  2,  4,  7,  8,  8,  2,  8,  7,  2,
		2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	}
};

float dist(float ax, float ay, float bx, float by) {
	return sqrtf(powf(bx - ax, 2) + powf(by - ay, 2));
}

int main()
{
	// Setup console window
	CHAR_INFO* pBuffer = new CHAR_INFO[nScreenWidth * nScreenHeight];
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(hConsole, { nScreenWidth,nScreenHeight });
	SetConsoleWindowInfo(hConsole, TRUE, new SMALL_RECT{ 0, 0, nScreenWidth - 1, nScreenHeight - 1 });
	SetConsoleCursorInfo(hConsole, new CONSOLE_CURSOR_INFO{ 10, FALSE });

	// Setup timers
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();
	auto tpStart = chrono::system_clock::now();

	// Main loop
	while (true) {
		// Calculate elapsed time
		tp2 = chrono::system_clock::now();
		chrono::duration<float> dElapsedDuration = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = dElapsedDuration.count();
		float fRunTime = (tp1 - tpStart).count();

		// Controls		
		// Turn left
		if (GetAsyncKeyState(0x25)) {
			fPlayerA += fPlayerRotation * fElapsedTime; if (fPlayerA >= 2 * PI) { fPlayerA -= 2 * PI; }
		}
		// Turn right
		if (GetAsyncKeyState(0x27)) {
			fPlayerA -= fPlayerRotation * fElapsedTime; if (fPlayerA <= 0) { fPlayerA += 2 * PI; }
		}
		// Strafe left
		if (GetAsyncKeyState(0x41)) {
			if (Map[(int)fPlayerY * nMapWidth + (int)(fPlayerX - fPlayerA / abs(fPlayerA) * fPlayerRadius)] == 0) { fPlayerX -= fPlayerSpeed * sinf(fPlayerA) * fElapsedTime; }
			if (Map[(int)(fPlayerY - fPlayerA / abs(fPlayerA) * fPlayerRadius) * nMapWidth + (int)fPlayerX] == 0) { fPlayerY -= fPlayerSpeed * cosf(fPlayerA) * fElapsedTime; }
		}
		// Strafe right
		if (GetAsyncKeyState(0x44)) {
			if (Map[(int)fPlayerY * nMapWidth + (int)(fPlayerX + fPlayerA / abs(fPlayerA) * fPlayerRadius)] == 0) { fPlayerX += fPlayerSpeed * sinf(fPlayerA) * fElapsedTime; }
			if (Map[(int)(fPlayerY + fPlayerA / abs(fPlayerA) * fPlayerRadius) * nMapWidth + (int)fPlayerX] == 0) { fPlayerY += fPlayerSpeed * cosf(fPlayerA) * fElapsedTime; }
		}
		// Sprinting
		if (GetAsyncKeyState(0x10)) { bPlayerSprint = true; }
		else { bPlayerSprint = false; }
		// Move forward
		if (GetAsyncKeyState(0x57)) {
			if (Map[(int)fPlayerY * nMapWidth + (int)(fPlayerX + fPlayerA / abs(fPlayerA) * fPlayerRadius)] == 0) {
				fPlayerX += (bPlayerSprint * fPlayerSprint + fPlayerSpeed) * cosf(fPlayerA) * fElapsedTime;
			}
			if (Map[(int)(fPlayerY - fPlayerA / abs(fPlayerA) * fPlayerRadius) * nMapWidth + (int)fPlayerX] == 0) {
				fPlayerY -= (bPlayerSprint * fPlayerSprint + fPlayerSpeed) * sinf(fPlayerA) * fElapsedTime;
			}
		}
		// Move backward
		if (GetAsyncKeyState(0x53)) {
			if (Map[(int)fPlayerY * nMapWidth + (int)(fPlayerX - fPlayerA / abs(fPlayerA) * fPlayerRadius)] == 0) { fPlayerX -= fPlayerSpeed * cosf(fPlayerA) * fElapsedTime; }
			if (Map[(int)(fPlayerY + fPlayerA / abs(fPlayerA) * fPlayerRadius) * nMapWidth + (int)fPlayerX] == 0) { fPlayerY += fPlayerSpeed * sinf(fPlayerA) * fElapsedTime; }
		}
		// Interact
		if (GetAsyncKeyState(0x45)) {
			short nInteractX = (short)(fPlayerX + cosf(fPlayerA)), nInteractY = (short)(fPlayerY - sinf(fPlayerA));
			if (Map[nInteractY * nMapWidth + nInteractX] == 2) { Map[nInteractY * nMapWidth + nInteractX] = 0; }
		}
		//// Ascend
		//if (GetAsyncKeyState(0x20)) { fPlayerZ += fElapsedTime; }
		//// Descend
		//if (GetAsyncKeyState(0x11)) { fPlayerZ -= fElapsedTime; }

		// View bobbing
		fPlayerZ = fPlayerHeight + sinf((fRunTime * (1 + bPlayerSprint * 2))/5000000.0f)/70;

		// Raycasting
		for (short x = 0; x < nScreenWidth; x++) {
			float fRayX = fPlayerX, fRayY = fPlayerY, fOffsetX = 0, fOffsetY = 0, fRayDist;
			float fRayA = fPlayerA + fFOV / 2 - ((float)x / (float)nScreenWidth) * fFOV; if (fRayA > 2 * PI) { fRayA -= 2 * PI; } if (fRayA < 0) { fRayA += 2 * PI; } // Calculate ray angle
			bool bVert = false;
			short nWallId = 1;

			// Horizontal check
			short nDOF = 0;
			float fHX = fPlayerX, fHY = fPlayerY, fHDist = 100000;;
			// Looking up
			if (fRayA < PI) {
				fRayY = floorf(fPlayerY) - 0.000001f; fRayX = fPlayerX + (fPlayerY - fRayY) / tanf(fRayA); fOffsetY = -1; fOffsetX = 1 / tanf(fRayA);
			}
			// Looking down
			float a = tanf(fRayA);
			if (fRayA > PI) {
				fRayY = ceilf(fPlayerY); fRayX = fPlayerX + (fPlayerY - fRayY) / tanf(fRayA); fOffsetY = 1; fOffsetX = -1 / tanf(fRayA);
			}
			if (fRayA == 0 || fRayA == 2 * PI) { fRayX = 100000; fRayY = 100000; nDOF = nMapWidth; }
			// Check with map
			while (nDOF < nMapWidth) {
				if (fRayY * nMapWidth + fRayX > 0 && fRayY * nMapWidth + fRayX < nMapWidth * nMapHeight && Map[(int)fRayY * nMapWidth + (int)fRayX] > 0) {
					fHX = fRayX; fHY = fRayY; fHDist = dist(fPlayerX, fPlayerY, fHX, fHY); nDOF = nMapWidth;
				}
				else { fRayX += fOffsetX; fRayY += fOffsetY; nDOF += 1; }
			}

			// Vertical check
			nDOF = 0;
			float fVX = fPlayerX, fVY = fPlayerY, fVDist = 100000;;
			// Looking left
			if (fRayA > PI / 2 && fRayA < (3 * PI) / 2) {
				fRayX = floorf(fPlayerX) - 0.000001f; fRayY = fPlayerY + (fPlayerX - fRayX) * tanf(fRayA); fOffsetX = -1; fOffsetY = 1 * tanf(fRayA);
			}
			// Looking right
			if (fRayA < PI / 2 || fRayA > (3 * PI) / 2) {
				fRayX = ceilf(fPlayerX); fRayY = fPlayerY + (fPlayerX - fRayX) * tanf(fRayA); fOffsetX = 1; fOffsetY = -1 * tanf(fRayA);
			}
			if (fRayA == PI / 2 || fRayA == (3 * PI) / 2) { fRayX = 100000; fRayY = 100000; nDOF = nMapHeight; }
			// Check with map
			while (nDOF < nMapHeight) {
				if (fRayY * nMapWidth + fRayX > 0 && fRayY * nMapWidth + fRayX < nMapWidth * nMapHeight && Map[(int)fRayY * nMapWidth + (int)fRayX] > 0) {
					fVX = fRayX; fVY = fRayY; fVDist = dist(fPlayerX, fPlayerY, fVX, fVY); nDOF = nMapHeight;
				}
				else { fRayX += fOffsetX; fRayY += fOffsetY; nDOF += 1; }
			}

			// Set ray to shortest ray
			if (fHDist < fVDist) { fRayX = fHX; fRayY = fHY; bVert = false; }
			if (fVDist < fHDist) { fRayX = fVX; fRayY = fVY; bVert = true; }
			fRayDist = dist(fPlayerX, fPlayerY, fRayX, fRayY);

			// Fix fisheye effect
			float fFishEyeA = fPlayerA - fRayA; if (fFishEyeA < 0) { fFishEyeA += 2 * PI; } if (fFishEyeA > 2 * PI) { fFishEyeA -= 2 * PI; }
			fRayDist = fRayDist * cosf(fFishEyeA);

			// Get wall id
			if (fRayY * nMapWidth + fRayX > 0 && fRayY * nMapWidth + fRayX < nMapWidth * nMapHeight && Map[(int)fRayY * nMapWidth + (int)fRayX] > 0) {
				nWallId = Map[(int)fRayY * nMapWidth + (int)fRayX];
			}

			// Calculate wall variables
			short nLineHeight = (short)(fProjDist / fRayDist), nCeiling = ((nScreenHeight - nLineHeight) / 2) + nLineHeight * (fPlayerZ - 0.5), nFloor = nLineHeight + nCeiling;

			// Draw screen buffer
			for (short y = 0; y < nScreenHeight; y++) {
				// Determine if drawing ceiling, wall or floor
				if (y < nCeiling) {
					// Texture ceiling
					float fCeilingDist = ((1 - fPlayerZ) * fProjDist / (nScreenHeight / 2 - y)) / cosf(fFishEyeA); // Random bullshit go!
					float fCeilingX = fPlayerX + fCeilingDist * cosf(fRayA), fCeilingY = fPlayerY - fCeilingDist * sinf(fRayA);
					short nTextureX = (fCeilingX - floorf(fCeilingX)) * nTextureWidth, nTextureY = (fCeilingY - floorf(fCeilingY)) * nTextureHeight;
					float fShade = powf(fShadeFactor, fCeilingDist);
					pBuffer[y * nScreenWidth + x] = { Palette[(int)(Texture[0][nTextureY * nTextureWidth + nTextureX] / fShade)], 0x000b };
				}
				else if (y >= nCeiling && y <= nFloor) {
					// Texture walls
					short nTextureX = (short)((bVert ? fRayY - floorf(fRayY) : fRayX - floorf(fRayX)) * nTextureWidth);
					short nTextureY = (short)roundf((((float)y - (float)nCeiling) / ((float)nLineHeight / 2.0f)) * (nTextureHeight - 1)) % nTextureHeight;
					float fShade = powf(fShadeFactor, fRayDist);
					short nTextureColor = nWallId == 2 ? ColorPalette[5] : ColorPalette[2];
					CHAR_INFO TextureCharInfo = { Palette[(int)(Texture[nWallId - 1][nTextureY * nTextureWidth + nTextureX] / fShade)], nTextureColor | (bVert * 0b1000)};
					pBuffer[y * nScreenWidth + x] = TextureCharInfo;
				}
				else {
					// Texture floor
					float fFloorDist = ((fPlayerZ) * fProjDist / (y - (nScreenHeight / 2))) / cosf(fFishEyeA); // Random bullshit go!
					float fFloorX = fPlayerX + fFloorDist * cosf(fRayA), fFloorY = fPlayerY - fFloorDist * sinf(fRayA);
					short nTextureX = (fFloorX - floorf(fFloorX)) * nTextureWidth, nTextureY = (fFloorY - floorf(fFloorY)) * nTextureHeight;
					float fShade = powf(fShadeFactor, fFloorDist);
					pBuffer[y * nScreenWidth + x] = { Palette[(int)(Texture[0][nTextureY * nTextureWidth + nTextureX] / fShade)], 0b0111};
				}
			}
		}

		wchar_t* pInfoA = new wchar_t[100];
		int nSize = swprintf_s(pInfoA, 100, L"X: %f Y: %f Z: %f A: %f FPS: %f", fPlayerX, fPlayerY, fPlayerZ, fPlayerA / (PI / 180), 1.0f / fElapsedTime);
		wchar_t* pInfo = new wchar_t[nSize];
		swprintf(pInfo, nSize, L"X: %f Y: %f Z: %f A: %f FPS: %f", fPlayerX, fPlayerY, fPlayerZ, fPlayerA / (PI / 180), 1.0f / fElapsedTime);
		for (short i = 0; i < nSize; i++) {
			pBuffer[i] = { pInfo[i], 0b0111 };
		}

		// Write buffer to console
		WriteConsoleOutput(hConsole, pBuffer, { nScreenWidth,nScreenHeight }, { 0,0 }, new SMALL_RECT{ 0, 0, nScreenWidth, nScreenHeight });
	}
}
