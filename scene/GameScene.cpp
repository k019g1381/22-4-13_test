#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

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
	//
	audio_->PlayWave(soundDataHundle_);

	//音声再生
	voiceHundle_ = audio_->PlayWave(soundDataHundle_, true);

	//スプライトの生成
	sprite_ = Sprite::Create(textureHundle_, {100, 50});
	model_ = Model::Create();

	worldTransfrom_.Initialize();
	viewRrojection_.Initialize();
}

void GameScene::Update() {
	//スプライトの今の座標を習得
	XMFLOAT2 position = sprite_->GetPosition();
	//座標2.0移動
	position.x += 2.0f;
	position.y += 1.0f;

	//スプライトの移動
	sprite_->SetPosition(position);

	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(voiceHundle_);
	}

	//デバッグテキストの表示
	// debugText_->Print("Kogakuin ni oreha iru.",50, 30, 1.0f);

	//書式指定付き表示
	// debugText_->SetPos(50,70);
	// debugText_->Printf(" year:%d ", 2001);

	//変数の値をインクリメント
	value_++;

	//値を含んだ文字列
	std::string strDebug = std::string("Value:") + std::to_string(value_);

	//デバッグテキストの表示
	debugText_->Print(strDebug, 50, 50, 1.0f);
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
	model_->Draw(worldTransfrom_, viewRrojection_, textureHundle_);
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
	sprite_->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
