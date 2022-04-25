#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	//デストラクタ
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//テクスチャハンドルを指定して読み込む
	textureHundle_ = TextureManager::Load("mario.jpg");
	soundDataHundle_ = audio_->LoadWave("se_sad03.wav");
	
	

	//課題1-2
	////音声再生
	// audio_->PlayWave(soundDataHundle_);
	//voiceHundle_ = audio_->PlayWave(soundDataHundle_, true);

	//スプライトの生成
	sprite_ = Sprite::Create(textureHundle_, {100, 50});
	model_ = Model::Create();

	
	//乱数シード生成器
	 std::random_device seed_gen;
	//メルセンスツイスター
	 std::mt19937_64 engine(seed_gen());

	//乱数範囲
	 std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	 std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	 // x,y,z軸回りの平行移動を設定
	 /*worldTransform_.translation_ = {0.0f, 10.0f, 0.0f};	
	 worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	 worldTransform_.rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, 0.0f};
	 worldTransform_.translation_ = {10.0f, 10.0f, 10.0f};*/

	 for (size_t i = 0; i < _countof(worldTransform_); i++) {

		 worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		 worldTransform_[i].rotation_ = {rotDist(engine),rotDist(engine),rotDist(engine)};		 
		 worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		 worldTransform_[i].Initialize();
	 }

	 //カメラ視点座標を設定
	 viewProjection_.eye = {0, 0, -20};

	 //カメラ注視点座標を設定
	 viewProjection_.target = {10, 0, 0};

	 //カメラの上方向ベクトルを設定
	 viewProjection_.up = {cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0.0f};

	 //ビュープロジェクションの初期化
	 //viewProjection_.Initialize(dxCommon_->GetDevice());
	 viewProjection_.Initialize();
}

void GameScene::Update() {

	//課題1-2
	////スプライトの今の座標を習得
	//XMFLOAT2 position = sprite_->GetPosition();
	////座標2.0移動
	//position.x += 2.0f;
	//position.y += 1.0f;
	//スプライトの移動
	//sprite_->SetPosition(position);

	//課題１－２
	////スペースキーを押した瞬間
	//if (input_->TriggerKey(DIK_SPACE)) {
	//	audio_->StopWave(voiceHundle_);
	//}
	//デバッグテキストの表示
	// debugText_->Print("Kogakuin ni oreha iru.",50, 30, 1.0f);
	//書式指定付き表示
	// debugText_->SetPos(50,70);
	// debugText_->Printf(" year:%d ", 2001);
	////変数の値をインクリメント
	//value_++;

	////値を含んだ文字列
	//std::string strDebug = std::string("Value:") + std::to_string(value_);

	////デバッグテキストの表示
	//debugText_->Print(strDebug, 50, 50, 1.0f);



	//2-2
	//視点移動処理
	{
		//視点の移動ベクトル
		XMFLOAT3 move = {0, 0, 0};

		//視点移動の早さ
		const float kEyeSpeed = 0.2f;
		//注視点移動の速さ
		const float KTargetSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) 
		{
			move = {-KTargetSpeed,0, 0};

		} else if (input_->PushKey(DIK_RIGHT))
		{
			move = {KTargetSpeed,0, 0};
		}

		if (input_->PushKey(DIK_W)) {
			move = {0, 0, kEyeSpeed};

		} else if (input_->PushKey(DIK_S)) {
			move = {0, 0, -kEyeSpeed};
		}


		//視点移動（ベクトルの加算）
		viewProjection_.eye.x += move.x;
		viewProjection_.eye.y += move.y;
		viewProjection_.eye.z += move.z;

		viewProjection_.target.x += move.x;
		viewProjection_.target.y += move.y;
		viewProjection_.target.z += move.z;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用の表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	}

	//上方向の回転処理
	{
		//上方向の回転の速さ[ラジアン/フレーム]
		const float KUProtSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_SPACE))
		{
			viewAngle += KUProtSpeed;
			//2πを越えたら元に戻る
			viewAngle = fmodf(viewAngle, XM_2PI);
		}

		//上方向ベクトルを計算（半径１円周上の座標）
		viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

		//デバッグテキストの表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
		  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y,
		  viewProjection_.up.z);

		//行列の再計算
		viewProjection_.UpdateMatrix();

	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	//model_->Draw(worldTransform_, viewProjection_, textureHundle_);
	for (size_t i = 0; i < _countof(worldTransform_); i++) 
	{
		model_->Draw(worldTransform_[i], viewProjection_, textureHundle_);
	}
	
	/// </summary>
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//sprite_->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	/*debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "translation:(%f,%f,%f)", worldTransform_].translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.y);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	  worldTransform_.rotation_.y);

	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "translation:(%f,%f,%f)", worldTransform_.scale_.x, worldTransform_.scale_.y,
	  worldTransform_.scale_.y);*/
	
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
