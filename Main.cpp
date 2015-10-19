/*
Main.cpp
エントリーポイント
メインループ

15/05/27
Naoki Nakagawa
*/
#include "vtx.h"
#include "Player.h"
#include "Stage.h"

// ゲームモード
enum GameMode
{
	title,
	main,
	clear,
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace vtx;
	GameMode mode = GameMode::main;
	int bg[3] = { 128, 192, 255 };
	bool clearFlag = false;
	int stageIndex = 0;
	std::unique_ptr<Lib> lib(new Lib(_T("ぷにぷに"), 800, 600, false));
	std::unique_ptr<Camera> camera(new Camera(D3DXVECTOR3(0, 0, -20), D3DXVECTOR3(0, 0, 0)));
	std::unique_ptr<Player> player(new Player());
	std::unique_ptr<Stage> stage(new Stage());
	std::unique_ptr<Particle> fireworks(new Particle());
	std::unique_ptr<Texture> fireParticles(new Texture(_T("Texture/Effect.png")));
	ShowCursor(false);

	while (app->wnd->MessageLoop())
	{
		render->Start();
		render->Clear(D3DCOLOR_XRGB(bg[0], bg[1], bg[2]));

		switch (mode)
		{
		case GameMode::title:
		{
			static std::unique_ptr<Mesh> logo(new Mesh(_T("Mesh/Logo.x")));
			static std::unique_ptr<Mesh> start(new Mesh(_T("Mesh/Start.x")));
			std::unique_ptr<Texture> pinkTexture(new Texture(_T("Texture/Pink.png")));
			static D3DXVECTOR3 scale = D3DXVECTOR3(1, 1, 1);
			static D3DXVECTOR3 scaleSp = D3DXVECTOR3(0, 0, 0);
			static bool isGameMain = false;	// ゲームメインに遷移することが決定したかどうか

			if (input->mouse->GetDownButton(0))
			{
				isGameMain = true;
				scale.x = 1.5f;
				scale.y = 1.5f;
			}

			if (isGameMain == true)
			{
				static int gameMainCount = 60;	// ゲームメインに遷移するカウント

				if (gameMainCount <= 0)
				{
					isGameMain = false;
					gameMainCount = 60;
					mode = GameMode::main;
				}

				gameMainCount--;
			}

			// 形状を戻す
			scaleSp.x += (1.0f - scale.x) * 0.5f;
			scaleSp.y += (1.0f - scale.y) * 0.5f;
			scaleSp.z += (1.0f - scale.z) * 0.5f;
			scale += scaleSp *= 0.75f;	// 数値は反発の値。0が低反発。1が高反発

			logo->Draw(&D3DXVECTOR3(0, 0.5f, -17.5f), &D3DXVECTOR3(0, 0, 0), &scale, pinkTexture->Get());
			start->Draw(&D3DXVECTOR3(0, -0.5f, -15), &D3DXVECTOR3(0, 0, 0), &scale, pinkTexture->Get());

			break;
		}
		case GameMode::main:
		{
			bg[0] += (128 - bg[0]) * 0.1f;
			bg[1] += (192 - bg[1]) * 0.1f;
			bg[2] += (255 - bg[2]) * 0.1f;
			
			// カメラワーク
			camera->pos.x += (player->backPos.x - camera->pos.x) * 0.1f;
			camera->pos.y += (player->backPos.y - camera->pos.y) * 0.1f;
			camera->pos.z = -20.0f;
			camera->View();

			// プレイヤー
			player->Update(*stage.get(), clearFlag);
			player->Draw();

			// ステージ
			char str[256];
			sprintf_s(str, "Stage/%03d.csv", stageIndex);
			static int b = -1; b++; if (b % 100 == 0) stage->Load(str);
			stage->Draw();

			if (clearFlag)
				mode = GameMode::clear;

			break;
		}
		case GameMode::clear:

			// 花火を打つ
			if (!math->Random(0, 30))
			{
				D3DXVECTOR3 tempPos(math->Random(-30.0f, 30.0f), math->Random(-30.0f, 30.0f), math->Random(50.0f, 100.0f));

				for (int i = 0; i < 100; i++)
				{
					D3DXVECTOR3 tempVec(math->Random(-1.0f, 1.0f), math->Random(-1.0f, 1.0f), math->Random(-1.0f, 1.0f));
					D3DXVec3Normalize(&tempVec, &tempVec);
					
					fireworks->Set(
						player->backPos + tempPos,
						tempVec * math->Random(0.5f, 1.0f),
						-0.03f,
						1.0f,
						math->Random(60, 100));
				}
			}

			bg[0] += (32 - bg[0]) * 0.1f;
			bg[1] += (64 - bg[1]) * 0.1f;
			bg[2] += (128 - bg[2]) * 0.1f;

			camera->pos.x += (player->backPos.x - camera->pos.x) * 0.1f;
			camera->pos.y += (player->backPos.y - camera->pos.y) * 0.1f;
			camera->pos.z = -20.0f;
			camera->View();

			player->Update(*stage.get(), clearFlag);
			player->Draw();

			stage->Draw();

			if (input->mouse->GetDownButton(0))
			{
				player->backPos = D3DXVECTOR3(0, 0, 0);
				player->frontPos = D3DXVECTOR3(0, 0, 0);
				mode = GameMode::main;
				stageIndex++;
				clearFlag = false;
				char str[256];
				sprintf_s(str, "Stage/%03d.csv", stageIndex);
				stage->Load(str);
				Sleep(100);
			}

			break;
		}

		fireworks->Draw(fireParticles->Get());

		render->End();
	}

	return 0;
}
