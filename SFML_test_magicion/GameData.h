#pragma once
#include "SFML_Custom.h"
using namespace Collision;
using namespace SFML_Custom;

namespace GameClass
{
#pragma region [�ý��� Ŭ���� : ����]
	class EntityManager;
	class SoundManager;
	class WorldManager;
	class InputManager;
	class CameraManager;
	class UiManager;
	class LevelManager;

	EntityManager* em;	//�浹 �Ŵ��� ����
	SoundManager* sm;	//���� �Ŵ��� ����.
	WorldManager* wm;	//���� �Ŵ��� ����
	InputManager* im;	//�Է� �Ŵ��� ����
	CameraManager* cm;	//ī�޶� �Ŵ��� ����
	UiManager* um;		//UI�Ŵ��� ����
	LevelManager* lm;	//���� �Ŵ��� ����
#pragma endregion

#pragma region [�⺻ Ŭ����]
	//�÷��̾� ��ü
	class Player {
	public:
		Player(Vector2f pos = Vector2f(0, 0));
		Vector2f position;
		float direction;
		float speed = 200.0f;
		CircleShape maskObj = CircleShape(10.0f);
		CircleShape circleShadow = CircleShape(10.0f);
		CircleShape* mask;

		int hp = 5;

		const int invincibleTimeMax = 180;
		int invincibleTime = 0;

		void GetDamage();

		void DoProcess();
		void DrawProcess();
	};

	//�� ��ü
	class Enemy {
	public:
		Enemy(Vector2f pos, float hpMax = 100, int expPoint = 10, Color clrOrigin = Color::Red);
		virtual ~Enemy();
		Vector2f position;
		float hpMax, hpNow;
		int exp = 10;

		float direction = 0;
		float speed = 100.0f;
		CircleShape maskObj = CircleShape(10.0f);
		CircleShape* mask = &maskObj;

		Color clrOrigin;

		const float hitEffectTimerMax = 500.f;
		float hitEffectTimer = 0;

		const void SetHit();

		const virtual void DoProcess() {};
		const virtual void DrawProcess() {};

		bool IsExpired()
		{
			if (hpNow < 0) { return true; }
			return false;
		}

		const void ResetFarPosition();
	};

	//����ü (����ü�� ����ϳ� �浹 �Ǵܰ� �ΰ��ӿ� ������ �ִ� ��� ����)
	class Projectile {
	public:
		Projectile() {}
		Projectile(Vector2f pos, float dir, int lv);
		virtual ~Projectile() {};
		int level;
		int lifeTime = 100;

		float direction;
		float height = 0;
		bool isFriendly;
		//CircleShape maskObj = CircleShape(4.0f);
		float speed = 300.0f;
		float damage;

		void virtual LifeRefresh()
		{
			lifeTime--;
		}
		bool virtual IsExpired()
		{
			if (lifeTime < 0) { return true; }	//���� �ð� ���� ��
			if (hitCount == 0) { return true; }	//����� ������ ���ߵ��� ��
			return false;
		}

		const virtual void DoProcess() {};
		const virtual void DrawProcess() {};

		list<Enemy*> hittedList;	//������ �� ���
		int hitCount;	//���� ������ ��

		const virtual void HitProcess(Enemy* enemyT) {};
		const virtual void EffectProcess(Enemy* enemy) {};	//������ ȿ�� �����
		const virtual bool DoCheckCollision(Enemy* enemy) { return false; };
		const void CollisionProcess();
	};

	//��ƼŬ
	class Particle {
	public:
		Particle(Vector2f pos, int lifeValue = 30, float dir = 0)
			: position(pos), life(lifeValue), direction(dir) {}
		Vector2f position;
		float direction;
		int life;

		virtual const void DoProcess() {};
		virtual const void DrawProcess() {};
		virtual const bool IsExpired()
		{
			if (life <= 0) { return true; }
			return false;
		}
	};

	//����
	class Weapon
	{
	public:
		int level = 0;

		int timerOriMax = int(1024 * 1.0f);
		int timerMax = int(1024 * 1.0f);
		int timerNow = 0;

		void AddLevel() {
			level++;
			AddLevelEffect();
		}
		virtual void AddLevelEffect() {};

		void DoProcess()
		{
			if (level == 0) { return; }	//������ 0�̸� ����Ǵ� ��� X
			timerNow += 1024 / frameRatio;

			if (timerMax < timerNow)
			{
				timerNow -= timerMax;
				DoFire();
			}
		};
		const virtual void DoProcessAttach() {};
		const virtual void DrawProcess() {};

		virtual void DoFire() {};

		Weapon()
		{
			timerOriMax = timerMax;
			boxBorder.setFillColor(Color(80, 80, 80, 120));
			boxBorder.setOutlineColor(Color(220, 220, 220, 255));

			boxBorder.setOrigin(boxBorder.getSize() / 2.f);
			sprChooserIcon.setScale(Vector2f(.5f, .5f));
			sprChooserIcon.setOrigin(Vector2f(193 * sprChooserIcon.getScale().x, 193 * sprChooserIcon.getScale().y));
		}

		//���� ���� ī�� draw
		Text txtName, txtDescript, txtLevel, txtPressButton;
		Font font;
		String strName, strDesStart, strDesEnhance, strDesEnd;

		Sprite sprChooserIcon;
		RectangleShape boxBorder;
		Vector2f centerDraw, centerTarget;
		float chooseValue = 0.f;

		void InitiateChooseProcess(String name, String desStart, String desEnhance, String desEnd,
			string path)
		{
			font.loadFromFile("Jalnan.ttf");

			strName = name;
			strDesStart = desStart;
			strDesEnhance = desEnhance;
			strDesEnd = desEnd;

			txtName.setString(name);
			txtName.setFont(font);
			txtName.setCharacterSize(40.f);
			txtName.setOutlineColor(Color::Black);
			txtName.setOutlineThickness(2.f);

			txtDescript.setString(strDesStart);
			txtDescript.setFont(font);
			txtDescript.setCharacterSize(20.f);
			txtDescript.setOutlineColor(Color::Black);
			txtDescript.setOutlineThickness(2.f);

			txtLevel.setString("Lv. " + to_string(level + 1));
			txtLevel.setFont(font);
			txtLevel.setCharacterSize(40.f);
			txtLevel.setOutlineColor(Color::Black);
			txtLevel.setOutlineThickness(2.f);

			txtPressButton.setFont(font);
			txtPressButton.setCharacterSize(40.f);
			txtPressButton.setOutlineColor(Color::Black);
			txtPressButton.setOutlineThickness(2.f);

			string sprName = "chooser" + to_string(random(0, 999));

			if (!LoadTexture(path, sprName)) { timerMax = 1; }
			sprChooserIcon = MakeSprite(sprName);
			sprChooserIcon.setScale(0.4f, 0.4f);
			FloatRect bounds = sprChooserIcon.getLocalBounds();
			sprChooserIcon.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

			boxBorder.setSize(Vector2f(400.f, 600.f));
			boxBorder.setOrigin(Vector2f(boxBorder.getSize().x / 2.f, boxBorder.getSize().y / 2.f));
			boxBorder.setOutlineColor(Color::White);
			boxBorder.setOutlineThickness(5.0f);
		}

		void ResetChooser(int level, int slot)
		{
			//txt �ʱ�ȭ
			switch (level)
			{
			case 0:
				txtDescript.setString(String(strDesStart));
				break;
			case 9:
				txtDescript.setString(strDesEnd);
				break;
			default:
				txtDescript.setString(strDesEnhance);
			}
			txtName.setString(strName);
			txtLevel.setString(to_string(level + 1));
			txtPressButton.setString("Press " + to_string(slot + 1) + " to choose!");

			//slot�� �°� centerTarget ��ġ �ʱ�ȭ
			centerTarget = Vector2f((slot - 1.f) * 500.f + screenSize.x / 2.f, screenSize.y / 2.f);
		}

		void DoChooserProcess();

		void DrawChooserProcess()
		{
			nowWindow->draw(boxBorder);
			nowWindow->draw(sprChooserIcon);
			nowWindow->draw(txtLevel);
			nowWindow->draw(txtName);
			nowWindow->draw(txtDescript);
			nowWindow->draw(txtPressButton);
		}

	};
#pragma endregion

#pragma region [��ƼŬ - �ڽ�]

	class BulletPart : public Particle
	{
	public:
		BulletPart(Vector2f pos, float dir) : Particle(pos, 60), direction(dir + random(-40.f, 40.f))
		{
			vSpeed = random(75.0f, 135.0f);
			height = 2.0f;
			hSpeed = random(-1.0f, 1.0f);
			gravity = -0.0f;
			circleDraw.setFillColor(Color((Color::Yellow.r + Color::Red.r) / 2, (Color::Yellow.g + Color::Red.g) / 2, (Color::Yellow.b + Color::Red.b) / 2));

			circleShadow.setFillColor(Color(Color::Black.r, Color::Black.g, Color::Black.b, Uint8(150)));
			circleShadow.setScale(Vector2f(1.0f, 0.5f));
		}

		CircleShape circleDraw = CircleShape(2.0f);
		CircleShape circleShadow = CircleShape(2.0f);
		float vSpeed, hSpeed, height, gravity, direction;

		const void DoProcess() override
		{
			position += GetMoveDirection(direction, vSpeed / 60.0f);
			height += hSpeed;
			hSpeed += gravity;
			circleDraw.setRadius(life / 60.f * 2.f);
			circleShadow.setRadius(life / 60.f * 2.f);
			circleDraw.setPosition(position);
			life--;
		}
		const void DrawProcess() override;
		const bool IsExpired() override
		{
			if (life <= 0) { return true; }
			if (height <= 0) { return true; }
			return false;
		}
	};

	class GutsPart : public Particle
	{
	public:
		GutsPart(Vector2f pos, float dir) : Particle(pos, 120), direction(random(0.f, 360.f))
		{
			vSpeed = pow(random(0.0f, 10.0f), 2.f);
			height = 5.f;
			hSpeed = random(-3.0f, 13.0f);
			gravity = -0.4f;
			circleDraw.setFillColor(Color::Red);
			size = random(0.3f, 2.f);

			circleShadow.setFillColor(Color(Color::Black.r, Color::Black.g, Color::Black.b, Uint8(150)));
			circleShadow.setScale(Vector2f(1.0f, 0.5f));
			circleDraw.setRadius(size);
			circleShadow.setRadius(size);
		}

		CircleShape circleDraw = CircleShape(2.0f);
		CircleShape circleShadow = CircleShape(2.0f);
		float vSpeed, hSpeed, height, gravity, direction, size;

		const void DoProcess() override
		{
			if (height > 0) {
				position += GetMoveDirection(direction, vSpeed / 60.0f);
				height += hSpeed;
				hSpeed += gravity;
			}
			circleDraw.setFillColor(Color(circleDraw.getFillColor().r, circleDraw.getFillColor().g, circleDraw.getFillColor().b, Uint8(255 * life / 120)));
			circleShadow.setFillColor(Color(circleDraw.getFillColor().r, circleDraw.getFillColor().g, circleDraw.getFillColor().b, Uint8(255 * life / 240)));
			life--;
		}
		const void DrawProcess() override;
		const bool IsExpired() override
		{
			if (life <= 0) { return true; }
			return false;
		}
	};

	class RayPart : public Particle
	{
	public:
		RayPart(Vector2f pos, float dir) : Particle(pos, 60), direction(dir)
		{
			vSpeed = random(75.0f, 135.0f);
			height = 2.0f;
			hSpeed = random(-1.0f, 1.0f);
			gravity = -0.0f;
			circleDraw.setFillColor(Color(180, 180, 255));

			circleShadow.setFillColor(Color(Color::Black.r, Color::Black.g, Color::Black.b, Uint8(150)));
			circleShadow.setScale(Vector2f(1.0f, 0.5f));
		}

		CircleShape circleDraw = CircleShape(2.0f);
		CircleShape circleShadow = CircleShape(2.0f);
		float vSpeed, hSpeed, height, gravity, direction;

		const void DoProcess() override
		{
			position += GetMoveDirection(direction, vSpeed / 60.0f);
			height += hSpeed;
			hSpeed += gravity;
			circleDraw.setRadius(life / 60.f * 2.f);
			circleShadow.setRadius(life / 60.f * 2.f);
			circleDraw.setPosition(position);
			life--;
		}
		const void DrawProcess() override;
		const bool IsExpired() override
		{
			if (life <= 0) { return true; }
			if (height <= 0) { return true; }
			return false;
		}
	};

	class MissileTailPart : public Particle
	{
	public:
		MissileTailPart(Vector2f pos, float dir) : Particle(pos, 25), direction(dir)
		{
			vSpeed = random(10.0f, 25.0f);
			height = 2.0f;
			hSpeed = random(-0.5f, 0.5f);
			circleDraw.setFillColor(Color(180, 180, 180));
			circleDraw.setOrigin(circleDraw.getRadius(), circleDraw.getRadius());
			lifeMax = life;
		}

		CircleShape circleDraw = CircleShape(4.0f);
		float vSpeed, hSpeed, height, direction;
		int lifeMax;

		const void DoProcess() override
		{
			life--;

			position += GetMoveDirection(direction, vSpeed / 60.0f);
			height += hSpeed;

			float lifeRatio = (float)life / lifeMax;
			circleDraw.setPosition(position);
			circleDraw.setFillColor(Color(180, 180, 180, 255 * lifeRatio));
		}
		const void DrawProcess() override;
		const bool IsExpired() override
		{
			if (life <= 0) { return true; }
			if (height <= 0) { return true; }
			return false;
		}
	};

#pragma endregion

#pragma region [���� - �ڽ�]

	class HandGun : public Weapon
	{
	public:
		HandGun() {
			timerOriMax = int(1024 * 0.7f);
			timerMax = timerOriMax;

			InitiateChooseProcess(String(L"�ǽ���"),
				String(L"���� ���� �ɷ�ġ�� ��� ��Ȳ���� ����"),
				String(L"���� �ӵ� ���"),
				String(L"���� : �뱸��ź�� ������ ���ط��� ����"),
				"undefinedIcon.png");
		}
		void AddLevelEffect() override
		{
			timerMax = timerOriMax * (1.f - 0.06f * (level - 1.f));
		}
		void DoFire();
	};

	class ShotGun : public Weapon
	{
	public:
		int timeFromFire = 100;

		ShotGun() {
			timerMax = int(1024 * 1.6f);
			InitiateChooseProcess(String(L"����"),
				String(L"�������� ġ������ ������ ������"),
				String(L"���� �ӵ� ���\n����ü �� ����"),
				String(L"���� : ���� �跲�� ������ �������� �߻�"),
				"undefinedIcon.png");
		}
		void AddLevelEffect() override
		{
			timerMax = timerOriMax * (1.f - 0.03f * (level - 1.f));
		}
		void DoFire();
		const void DoProcessAttach() override;
	};

	class SubMachineGun : public Weapon
	{
	public:

		int magMax = 30, magNow = 30;
		bool isReloading = false;
		const int timerMaxReload = int(1024 * 0.07f);
		const int timerMaxFire = int(1024 * 0.1f);

		SubMachineGun() {
			timerOriMax = int(1024 * 0.1f);
			timerMax = timerOriMax;

			InitiateChooseProcess(String(L"�������"),
				String(L"������ ������ ź�� ��Ѹ��� ����� ����"),
				String(L"��ȿ ��Ÿ� ����\nźâ ����\n"),
				String(L"���� : ��Ŵ���� ������ �ѹ��� �ι߾� �߻�"),
				"undefinedIcon.png");
		}
		void AddLevelEffect() override
		{
			magMax += 3;
		}
		void DoFire();
	};

	class RailGun : public Weapon
	{
	public:
		float direction = 0;

		int fireCountMax = 1;
		int fireCountNow = 0;
		int fireTimeMax = 20;
		int fireTimeNow = 0;

		RailGun() {
			timerMax = int(1024 * 3.2f);

			InitiateChooseProcess(String(L"���ϰ�"),
				String(L"�������� ������ ���� �� ��θ� ����"),
				String(L"���� �ӵ� ���\n������ �� ����"),
				String(L"���� : 3���� ����"),
				"undefinedIcon.png");
		}
		void AddLevelEffect() override
		{
			timerMax = timerOriMax * (1.f - 0.05f * (level - 1.f));
			if (level == 10) {}
		}
		void DoFire();
		const void DoProcessAttach() override;
		const void DrawProcess() override;
	};

	class SpiralMissileLauncher : public Weapon
	{
	public:
		float direction = 0;

		int fireCountMax = 4;
		int fireCountNow = 0;
		int fireTimeMax = 10;
		int fireTimeNow = 0;

		SpiralMissileLauncher() {
			timerMax = int(1024 * 4.5f);

			InitiateChooseProcess(String(L"���� �̻���"),
				String(L"���� �����ϴ� ����ü�� �߻�"),
				String(L"���� �ӵ� ���\n���� ���� ����"),
				String(L"�ٿ���: �߻�ü�� 4������ 10���� �����մϴ�."),
				"undefinedIcon.png");
		}
		void AddLevelEffect() override
		{
			timerMax = timerOriMax * (1.f - 0.04f * (level - 1.f));
			if (level >= 10) {
				fireCountMax = 10;
				fireTimeMax = 6;
			}
		}
		void DoFire();
		const void DoProcessAttach() override;
	};

#pragma endregion

#pragma region [�ý��� Ŭ����]

	//�÷��̾� ��ü�� ���� ����
	class LevelManager
	{
		//������� ����� ��ųʸ�
		list<unique_ptr<Weapon>> WeaponList = list<unique_ptr<Weapon>>();
		list<Weapon*> weaponChoose = list<Weapon*>();
		Text txtChooseAnnounce;
		Font font;
	public:
		int playerLevel = 0;
		int nowExp = 0;
		int requireExp = 100;
		int stackedExp = 0;

		float expRatio = 0;

		LevelManager()
		{
			font.loadFromFile("Jalnan.ttf");
			txtChooseAnnounce.setFont(font);
			txtChooseAnnounce.setCharacterSize(50.f);
			txtChooseAnnounce.setFillColor(Color::White);
			txtChooseAnnounce.setOutlineColor(Color::Black);
			txtChooseAnnounce.setOutlineThickness(2.f);
			txtChooseAnnounce.setPosition(screenSize.x / 2.f, screenSize.y - 100.0f);
			txtChooseAnnounce.setString(String(L"���� ����! ( " + to_string(toChoose) + L" ȸ ����)"));
			sf::FloatRect bounds = txtChooseAnnounce.getLocalBounds();
			txtChooseAnnounce.setOrigin(bounds.width / 2, bounds.height / 2);

			toChoose++;

			requireExp = GetRequireExp(0);

			//�ڵ��
			HandGun* hg = new HandGun();
			WeaponList.push_back(unique_ptr<Weapon>(hg));
			//hg->AddLevel();	//����׿� ������

			//����
			ShotGun* sg = new ShotGun();
			WeaponList.push_back(unique_ptr<Weapon>(sg));
			//sg->AddLevel();	//����׿� ������

			//SMG
			SubMachineGun* smg = new SubMachineGun();
			WeaponList.push_back(unique_ptr<Weapon>(smg));
			//smg->AddLevel();	//����׿� ������

			//���ϰ�
			RailGun* rg = new RailGun();
			WeaponList.push_back(unique_ptr<Weapon>(rg));
			//rg->AddLevel();	//����׿� ������

			//�̻���
			SpiralMissileLauncher* ml = new SpiralMissileLauncher();
			WeaponList.push_back(unique_ptr<Weapon>(ml));
			//ml->AddLevel();	//����׿� ������
		}

		//���� ����
		int toChoose = 0;
		bool isChoosing = false;

		void InitiateChooseWeapon()
		{
			txtChooseAnnounce.setString("���⸦ �����ϼ���! (" + to_string(toChoose) + "ȸ ����)");

			isChoosing = true;
			weaponChoose.clear();

			std::unordered_set<int> chosenIndices; // ���õ� �ε����� �����ϱ� ���� unordered_set
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, WeaponList.size() - 1);

			while (weaponChoose.size() < 3)
			{
				int randomIndex = dis(gen);
				if (chosenIndices.find(randomIndex) == chosenIndices.end()) // �ߺ��� �ε����� �ƴ� ��쿡�� ����
				{
					chosenIndices.insert(randomIndex);
					auto it = WeaponList.begin();
					std::advance(it, randomIndex);
					weaponChoose.push_back(it->get());
				}
			}

			int slot = 0;
			for (auto& wp : weaponChoose)
			{
				wp->ResetChooser(wp->level, slot);
				slot++;
			}
		}

		void DrawChooseWeapon()
		{
			for (const auto& weapon : weaponChoose)
			{
				weapon->DrawChooserProcess();
			}

			txtChooseAnnounce.setString(sf::String(L"���� ����! (" + to_string(toChoose) + L"ȸ ����)"));
			nowWindow->draw(txtChooseAnnounce);

		}
		void DoChooseWeapon()
		{
			//�Է� �޾ƿ�
			bool firstPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1);
			bool secondPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Num2);
			bool thirdPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Num3);

			//�Է� > Ÿ�� ����
			int tSlot = -1;
			if (firstPressed) { tSlot = 0; }
			if (secondPressed) { tSlot = 1; }
			if (thirdPressed) { tSlot = 2; }

			//�Է� ���� �� ����
			if (tSlot == -1) { return; }

			//�Է� ���� �� �Է¿� ���� DeciseChooseWeapon()ȣ��
			int nowSlot = 0;
			for (auto& weapon : weaponChoose)
			{
				if (tSlot == nowSlot) { DeciseChooseWeapon(weapon); }
				nowSlot++;
			}
		}

		void DeciseChooseWeapon(Weapon* weapon)
		{
			weapon->AddLevel();

			isChoosing = false;
			toChoose--;
		}


		//���������� ����� �ڵ�
		void WeaponProcess() //�������� ������� �̿��� ������ �����մϴ�.
		{
			for (const auto& weapon : WeaponList)
			{
				weapon->DoProcess();
				weapon->DoProcessAttach();
				weapon->DoChooserProcess();
			}
		}
		void DrawProcess() //Draw�Ұ� �ִٸ� Draw
		{
			for (const auto& weapon : WeaponList)
			{
				weapon->DrawProcess();
			}

			if (isChoosing) { DrawChooseWeapon(); }
		}
		void LevelProcess() //������ ����ġ�� ���õ� step���μ���
		{
			//���� ����!
			if (isChoosing) { DoChooseWeapon(); }

			//����ġ �� �ʱ�ȭ
			expRatio = (float)nowExp / (float)requireExp;

			//����ġ �� ���� ������
			if (nowExp > requireExp) { LevelUp(); }

			//���� ���� �����ϸ� ���� ����
			if (toChoose != 0 && !isChoosing) { InitiateChooseWeapon(); }
		}

		//�׶� �׶� ����� �ڵ��
		int GetRequireExp(int level)
		{
			int valueOfLevel = pow(level + 5, 2.f);
			return (int)valueOfLevel * 10;
		}	//���� �������� �䱸�Ǵ� exp���� ���� ����մϴ�.
		void LevelUp()//<ĳ����>�������� �����մϴ�.
		{
			//���� ���ñ� +1
			toChoose++;

			//�Ķ���� ����
			nowExp -= requireExp;
			playerLevel++;
			requireExp = GetRequireExp(playerLevel);
		}

		void GetExp(int exp) //����ġ�� ����ϴ�.
		{
			nowExp += exp;
			stackedExp += exp;
		}
	};

	//UI����
	class UiManager
	{
	public:
		Text textDebug;

		Text textTimer;	//������ ����� ǥ���մϴ�.
		Text textLevel;	//���� ǥ��
		Text textExp;	//����ġ ǥ��

		Text textHp;	//ü��

		RectangleShape barExp;	//����ġ ��
		float  barExpValue = 0;
		RectangleShape barExpBackground;	//����ġ ��

		bool isGameEnded = false;
		float gameEndValue = 0.0f;
		//���ӿ����� ���� TEXT
		Text textGameOver;
		Text textEndTime;
		Text textScore;

		RectangleShape screenBox;


		Font font;
		UiManager()
		{
			int expBarHeight = 50;

			string path = "Jalnan.ttf";
			font.loadFromFile(path);

			textDebug.setFont(font);
			textDebug.setPosition(Vector2f(10.0f, 80.0f));

#pragma region [�⺻ UI]
			textTimer.setFont(font);
			textTimer.setOutlineColor(Color::Black);
			textTimer.setOutlineThickness(2.f);
			textTimer.setPosition(screenSize.x / 2.f - 65.f, 0);
			textTimer.setCharacterSize(expBarHeight - 10.f);

			textLevel.setFont(font);
			textLevel.setOutlineColor(Color::Black);
			textLevel.setOutlineThickness(2.f);
			textLevel.setPosition(screenSize.x / 2.f - 5.f, screenSize.y - expBarHeight - 5.f);
			textLevel.setCharacterSize(expBarHeight - 15.f);


			textHp.setFont(font);
			textHp.setOutlineColor(Color::Black);
			textHp.setOutlineThickness(3.f);
			textHp.setPosition(100.0f, 10.f);
			textHp.setCharacterSize(50.f);

			textExp.setFont(font);
			textExp.setOutlineColor(Color::Black);
			textExp.setOutlineThickness(1.f);
			textExp.setPosition(screenSize.x / 2.f - 50.f, screenSize.y - 20.f);
			textExp.setCharacterSize(15.0f);

			barExp = RectangleShape(Vector2f(screenSize.x, expBarHeight));
			barExp.setFillColor(Color(255, 255, Color::Blue.b * 0.5f));
			barExp.setOutlineColor(Color::Black);
			barExp.setPosition(0, screenSize.y - expBarHeight);
			barExp.setOutlineThickness(3.f);

			barExpBackground = RectangleShape(Vector2f(screenSize.x, expBarHeight));
			barExpBackground.setFillColor(Color(Color::White.r * 0.1f, Color::White.g * 0.1f, Color::White.b * 0.1f));
			barExpBackground.setOutlineColor(Color::Black);
			barExpBackground.setPosition(0, screenSize.y - expBarHeight);
			barExpBackground.setOutlineThickness(3.f);

#pragma endregion

#pragma region [���� ���� UI]

			screenBox.setSize((Vector2f)screenSize);
			screenBox.setFillColor(Color::Red);

			textGameOver.setString(sf::String(L"���� ����!"));
			textGameOver.setFont(font);
			textGameOver.setOutlineColor(Color::Black);
			textGameOver.setOutlineThickness(2.f);
			textGameOver.setPosition(screenSize.x / 2.f - 300.f, screenSize.y / 2.f - 200.f);
			textGameOver.setCharacterSize(150.f);

			textEndTime.setFont(font);
			textEndTime.setOutlineColor(Color::Black);
			textEndTime.setOutlineThickness(2.f);
			textEndTime.setPosition(screenSize.x / 2.f - 150.f, screenSize.y / 2.f - 000.f);
			textEndTime.setCharacterSize(40.f);

			textScore.setFont(font);
			textScore.setOutlineColor(Color::Black);
			textScore.setOutlineThickness(2.f);
			textScore.setPosition(screenSize.x / 2.f - 150.f, screenSize.y / 2.f + 50.f);
			textScore.setCharacterSize(40.f);

#pragma endregion

		}

		void UiProcess();

		void UiDraw()
		{
			//nowWindow->draw(textDebug);

			//����ġ �� ��ο�
			nowWindow->draw(barExpBackground);
			nowWindow->draw(barExp);

			nowWindow->draw(textTimer);
			nowWindow->draw(textLevel);
			nowWindow->draw(textExp);
			nowWindow->draw(textHp);
			if (isGameEnded) { GameOverDraw(); }
		}
		void SetDebugMessage(string txt) { textDebug.setString(txt); }

		void GameOverDraw()
		{
			float speed = 100.0f;
			gameEndValue = (gameEndValue * speed + 1.f) / (speed + 1.f);
			Color endColor = Color(255, 255, 255, Uint8(255 * gameEndValue));

			screenBox.setFillColor(Color(255, 0, 0, endColor.a / 2));
			textGameOver.setFillColor(endColor);
			textEndTime.setFillColor(endColor);
			textScore.setFillColor(endColor);

			nowWindow->draw(screenBox);
			nowWindow->draw(textGameOver);
			nowWindow->draw(textEndTime);
			nowWindow->draw(textScore);
		}


	};

	//�浹 �Ǵ� �� ��ü ����
	class EntityManager
	{
	public:
		Player playerEntity = Player(Vector2f(100.0f, 100.0f));
		list<unique_ptr<Enemy>> enemyEntityList = list<unique_ptr<Enemy>>();
		list<unique_ptr<Projectile>> projectileList = list<unique_ptr<Projectile>>();
		list<unique_ptr<Particle>> particleList = list<unique_ptr<Particle>>();

		EntityManager()
		{
			enemyEntityList.clear();
			projectileList.clear();
		}
		void CheckProjectile(auto& projectile, bool isFriendly);
		void CheckEnemySpace()
		{
			for (const auto& enemy1 : enemyEntityList)
			{
				for (const auto& enemy2 : enemyEntityList)
				{
					if (enemy1 == enemy2) { continue; }
					if (CheckCollision(*(enemy1->mask), *(enemy2->mask)))
					{
						CircleShape* circle1 = enemy1->mask;
						CircleShape* circle2 = enemy2->mask;

						float forceDir = GetDirection(circle1->getPosition(), circle2->getPosition());
						float forceScalraOffset = ((circle1->getRadius() + circle2->getRadius()) - GetDistance(circle1->getPosition(), circle2->getPosition())) / (circle1->getRadius() + circle2->getRadius());
						float forceScalra = 0.5f + forceScalraOffset * 2.0f;
						Vector2f forceVector = GetMoveDirection(forceDir, forceScalra);

						enemy1->position -= forceVector;
						enemy2->position += forceVector;
					}
				}
			}

		}
		void CheckEnemyHit()
		{
			if (playerEntity.invincibleTime > 0) { return; }	//������ �� �浹 ó�� �ǳʶ�

			for (const auto& enemy : enemyEntityList)
			{
				if (CheckCollision(enemy->maskObj, playerEntity.maskObj) == true)
				{
					playerEntity.GetDamage();
					return;
				}
			}
		}

		void AddEnemy(Enemy* enemy)
		{
			enemyEntityList.push_back(unique_ptr<Enemy>(enemy));
		}
		void AddProjectile(Projectile* proj)
		{
			projectileList.push_back(unique_ptr<Projectile>(proj));
		}
		void AddParticle(Particle* part)
		{
			if (timeToProcess > 100.f || particleList.size() > 100) { return; }

			particleList.push_back(unique_ptr<Particle>(part));
		}

		Enemy* GetClosestEnemy(Vector2f origin)
		{
			float length = 99999;
			Enemy* target = nullptr;
			for (const auto& enemy : enemyEntityList)
			{
				float lengthTemp = GetDistance(enemy.get()->position, origin);

				if (lengthTemp < length) {
					target = enemy.get();
					length = lengthTemp;
				}
			}
			return target;
		}

		void DoAllProcess() {
			if (playerEntity.hp <= 0) { return; }


			for (const auto& enemy : enemyEntityList) {
				enemy->DoProcess();
				enemy->ResetFarPosition();
			}

			for (const auto& proj : projectileList) {
				proj->DoProcess();
				proj->CollisionProcess();
				proj->LifeRefresh();
			}

			playerEntity.DoProcess();
			CheckEnemyHit();

			for (const auto& part : particleList) {
				part->DoProcess();
			}

			//�޸� ���� �ڵ�
			for (auto it = enemyEntityList.begin(); it != enemyEntityList.end(); )
			{
				if ((*it)->IsExpired())
				{
					// ���� Ÿ�Կ� �´� �Ҹ��� ȣ��
					auto* enemyPtr = it->release(); // unique_ptr���� ������ ����
					delete enemyPtr; // �޸� ����

					it = enemyEntityList.erase(it); // enemyEntityList���� ����
				}
				else
				{
					++it;
				}
			}
			for (auto it = projectileList.begin(); it != projectileList.end(); )
			{
				if ((*it)->IsExpired())
				{
					// ���� Ÿ�Կ� �´� �Ҹ��� ȣ��
					auto* projPtr = it->release(); // unique_ptr���� ������ ����
					delete projPtr; // �޸� ����

					it = projectileList.erase(it); // enemyEntityList���� ����
				}
				else
				{
					++it;
				}
			}
			for (auto it = particleList.begin(); it != particleList.end(); )
			{
				if ((*it)->IsExpired())
				{
					// ���� Ÿ�Կ� �´� �Ҹ��� ȣ��
					auto* partPtr = it->release(); // unique_ptr���� ������ ����
					delete partPtr; // �޸� ����

					it = particleList.erase(it); // enemyEntityList���� ����
				}
				else
				{
					++it;
				}
			}

		}
		void DrawAllProcess() {
			//Debug
			for (const auto& enemy : enemyEntityList) { enemy->DrawProcess(); }
			playerEntity.DrawProcess();
			for (const auto& part : particleList) { part->DrawProcess(); }
			for (const auto& proj : projectileList) { proj->DrawProcess(); }
		}
	};

	//ī�޶� ����
	class CameraManager {
	public:

		float shakeValue = 0.0f;
		Vector2f positionReal = Vector2f(0, 0);	//���� ��ǥ
		Vector2f positionShake = Vector2f(0, 0);	//ȭ�� ȿ��
		Vector2f positionResult = Vector2f(0, 0);	//��� ��ǥ


		Vector2f TranslatePosition(Vector2f pos)
		{
			Vector2f posCen = em->playerEntity.position;//���߾��� �� ��ǥ
			return pos - posCen + (Vector2f)screenSize / 2.f;
		}

		void RefreshCamera()
		{
			positionReal = (Vector2f)screenSize / 2.0f - em->playerEntity.position;
			shakeValue = shakeValue * 2.f / (3.f);

			positionShake = Vector2f(random(shakeValue, -shakeValue), random(shakeValue, -shakeValue));

			positionResult = positionReal + positionShake;
		}
	};

	//���� ���� �� �������� ������ ����
	class WorldManager
	{
		long long timePast = 0;	//���� ��� �ð�

		int phase = 0;	//���� ��� ����

		int phaseSwarmMaxArr[6] = { 150, 225, 350, 400, 500, 50 };
		int phaseSwarmDeltaArr[6] = { 3, 4, 5, 6, 8, 12 };
		int RefreshTimer()	//�ð��� ���ΰ�ħ
		{
			timePast = GetCurrentTimeMillis() - timeStart;

			for (int i = 0; i < phaseMax; i++) {
				if (timePast < (long long)1024 * phaseTimeArr[i])
				{
					if (phase != i + 1)
					{
						phase = i + 1;

						switch (phase)
						{
						case 2:
							hybridCount += 1;
							break;
						case 3:
							hybridCount += 2;
							break;
						case 4:
							hybridCount += 3;
							break;
						}
					}

					return phase;
				}
			}
			return -1;
		}
		//Ÿ�̸� �ʱ�ȭ

		int hybridCount = 0;

		float spawnValue = 0;

		enum EnemyCode
		{
			SWARMER,
			COLOSSUS,
			ILLUSION,	//
			ASSAULT,
			PIERCER,	//
			WARBRINGER,
			DARKONE,
			HYBRID,		//
			UNTALKABLE,	//
			ERROR
		};

		void SpawnEnemyByPhase();

		void SpawnProcess();
		//�� ���� �˰���

		void WorldColorProcess()
		{
			Color ultClr = worldColor;
			switch (phase)
			{
			case 1: ultClr = sf::Color(123, 123, 123);
				break;
			case 2: ultClr = sf::Color(105, 200, 108);
				break;
			case 3: ultClr = sf::Color(170, 170, 120);
				break;
			case 4: ultClr = sf::Color(193, 88, 88);
				break;
			case 5: ultClr = sf::Color(80, 80, 80);
				break;
			case 6: ultClr = sf::Color(40, 40, 40);
				break;
			default:
				ultClr = Color::Black;
			}

			float speed = 300.f;
			worldColor = Color((ultClr.r + worldColor.r * speed) / (1.f + speed),
				(ultClr.g + worldColor.g * speed) / (1.f + speed),
				(ultClr.b + worldColor.b * speed) / (1.f + speed));
		}
		//���� ����

	public:
		long long timeStart = 0;	//���� ���۽ð�

		const int phaseMax = 6;
		int phaseTimeArr[6] = { 150, 300, 450, 660, 900, 9999 };

		sf::Color worldColor = sf::Color(123, 123, 123);

		void StartTimer()
		{
			phase = 0;
			timePast = 0;

			timeStart = GetCurrentTimeMillis();
		}
		//���� ���� �ÿ� ����

		void WorldProcess()
		{
			RefreshTimer();
			SpawnProcess();
			WorldColorProcess();
		}
		//���� �� ���� ����
	};

	//�Է°��� ���� ����
	class InputManager {
	public:
		bool upPressed = false, rightPressed = false, leftPressed = false, downPressed = false;
		Vector2f mousePos = Vector2f(0, 0);
		bool clickPressed = false;

		void TotalProcess(Event event) {
			upPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
			downPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
			leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
			rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

			if (event.type == Event::KeyPressed)
			{
				switch (event.key.code)
				{


				case Keyboard::F1:
					TestFunctionF1();
					break;
				case Keyboard::F2:
					TestFunctionF2();
					break;
				case Keyboard::F3:
					TestFunctionF3();
					break;

				case Keyboard::Escape:
					nowWindow->close();
					break;

				}
			}
			else if (event.type == Event::MouseButtonPressed)
			{
				clickPressed = false;

				int mouseX = event.mouseButton.x;
				int mouseY = event.mouseButton.y;

				mousePos = Vector2f((float)mouseX, (float)mouseY);

				switch (event.mouseButton.button)
				{
				case Mouse::Left:
					clickPressed = true;
					break;
				case Mouse::Right:

					break;
				}
			}
		}

		void TestFunctionF1();
		void TestFunctionF2();
		void TestFunctionF3();
	};

	//ȿ������ ������� ����
	class SoundManager
	{
	private:
		Dictionary<string, string> soundDictionary;
		list<Sound*> soundList;
		list<SoundBuffer*> bufferList;
		int bufferChoosen = 0;
		bool bufferUsing[32];

	public:
		SoundManager()
		{
			for (int i = 0; i < 32; i++) { bufferList.push_back(new SoundBuffer); bufferUsing[i] = false; }

			LoadSound("SubMachineFire.ogg", "smgFire");
			LoadSound("SpiralMissileFly.ogg", "mlFire");
			LoadSound("SpiralMissileBoom.ogg", "mlboom");
			LoadSound("ShotGunFire.ogg", "sgFire");
			LoadSound("RailGunFire.ogg", "rgFire");
			LoadSound("PlayerHit.ogg", "playerHit");
			LoadSound("HandGunFire.ogg", "hgFire");
			LoadSound("EnemyHit.ogg", "enemyHit");
			LoadSound("EnemyDie.ogg", "enemyDie");
		}

		void LoadSound(const string& filePath, const string& audioName)
		{
			soundDictionary.Add(audioName, filePath);
		}

		SoundBuffer* GetBuffer()
		{
			int i = 0;
			for (SoundBuffer* buffer : bufferList)
			{
				bufferChoosen = i;
				if (bufferUsing[i] == false) { return buffer; }
				i++;
			}
			return nullptr;
		}

		void PlaySound(const string& audioName)
		{
			const string& path = (soundDictionary.Get(audioName));

			SoundBuffer* buffer = GetBuffer();
			if (buffer == nullptr)
			{
				um->SetDebugMessage("ssibal");
				return;
			}


			if (buffer->loadFromFile(path))
			{
				Sound* sound = new Sound(*buffer);

				sound->setBuffer(*buffer);
				sound->setVolume(100.f);
				sound->setLoop(false);
				sound->play();
				soundList.push_back(sound);
				bufferUsing[bufferChoosen] = true;
			}
			else
			{
				um->SetDebugMessage("ssibal");
			}
		}

		void ClearSoundList()
		{
			auto it = soundList.begin();
			while (it != soundList.end())
			{
				if ((*it)->getStatus() != sf::Sound::Playing)
				{
					(*it)->stop();
					int i = 0;
					for (SoundBuffer* buffer : bufferList)
					{
						if (buffer == (*it)->getBuffer()) { break; }
						i++;
					}

					bufferUsing[i] = false;


					delete (*it);
					it = soundList.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	};

	//�ý��� Ŭ���� �����ڵ�
	void ResetAllManager()
	{
		em = new EntityManager;	//�浹 �Ŵ��� ����
		wm = new WorldManager;	//���� �Ŵ��� ����
		im = new InputManager;	//�Է� �Ŵ��� ����
		cm = new CameraManager;	//ī�޶� �Ŵ��� ����
		um = new UiManager;		//UI�Ŵ��� ����
		lm = new LevelManager;	//
		sm = new SoundManager;	//���� �Ŵ��� ����.
	}
#pragma endregion

#pragma region [��ƼŬ �ڽ� : ����]
	const void BulletPart::DrawProcess()
	{
		Vector2f ultPos = cm->TranslatePosition(position - Vector2f(circleDraw.getRadius(), circleDraw.getRadius()));
		DrawCircle(circleShadow, ultPos.x, ultPos.y + 2.0f);
		DrawCircle(circleDraw, ultPos.x, ultPos.y - height);
	}
	const void GutsPart::DrawProcess()
	{
		Vector2f ultPos = cm->TranslatePosition(position);

		DrawCircle(circleDraw, ultPos.x, ultPos.y - max(height, 0.f));

		if (height > 0) {
			DrawCircle(circleShadow, ultPos.x, ultPos.y);
		}
	}
	const void RayPart::DrawProcess()
	{
		Vector2f ultPos = cm->TranslatePosition(position - Vector2f(circleDraw.getRadius(), circleDraw.getRadius()));
		DrawCircle(circleShadow, ultPos.x, ultPos.y + 2.0f);
		DrawCircle(circleDraw, ultPos.x, ultPos.y - height);
	}
	const void MissileTailPart::DrawProcess()
	{
		Vector2f ultPos = cm->TranslatePosition(position);
		DrawCircle(circleDraw, ultPos.x, ultPos.y);
	}

#pragma endregion

#pragma region [�� ��ü�� ����ü - �ڽ�]
	//�� ��ü
	class Swarmer : public Enemy
	{
	public:
		CircleShape circleDraw = CircleShape(10.0f);
		CircleShape circleShadow = CircleShape(10.0f);

		Swarmer(Vector2f pos) :Enemy(pos, 23.f, 13, Color::Red)
		{
			speed = 100.f;

			mask->setRadius(10.0f);

			circleDraw.setFillColor(clrOrigin);

			circleShadow.setFillColor(Color(Color::Black.r, Color::Black.g, Color::Black.b, Uint8(150)));
			circleShadow.setScale(Vector2f(1.0f, 0.5f));
		}
		~Swarmer()
		{
			for (int i = 0; i <= random(3, 6); i++)
			{
				GutsPart* part = new GutsPart(position, direction);
				em->AddParticle(part);
			}
			//Enemy::~Enemy();
		}

		const void DoProcess() override
		{
			if (hitEffectTimer > 0) {
				hitEffectTimer = hitEffectTimer - float(1024 / 60);
				hitEffectTimer = max(hitEffectTimer, 0.f);
				float oriValue = (1.f - hitEffectTimer / hitEffectTimerMax);
				float whiValue = hitEffectTimer / hitEffectTimerMax;
				Color tColor = Color(clrOrigin.r * oriValue + Color::White.r * whiValue,
					clrOrigin.g * oriValue + Color::White.g * whiValue,
					clrOrigin.b * oriValue + Color::White.b * whiValue);
				circleDraw.setFillColor(tColor);
			}
			else { circleDraw.setFillColor(clrOrigin); }

			Vector2f tPos = em->playerEntity.position;
			Vector2f mPos = mask->getPosition();

			float dir = GetDirection(mPos, tPos);

			float togoScalra = speed / frameRatio;
			Vector2f togoVec = GetMoveDirection(dir, togoScalra);
			position += togoVec;
			maskObj.setPosition(position);
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position - Vector2f(maskObj.getRadius(), maskObj.getRadius()));
			if (hitEffectTimer > 0)
			{
				float shakeValue = maskObj.getRadius() / 5.0f * hitEffectTimer / hitEffectTimerMax;
				Vector2f shakePos = GetMoveDirection(random(0, 360), random(shakeValue, -shakeValue));
				ultPos += shakePos;
			}
			DrawCircle(circleShadow, ultPos.x, ultPos.y + 17.5f, 0, 0, circleShadow.getScale(), circleShadow.getFillColor());
			DrawCircle(circleDraw, ultPos.x, ultPos.y);
		}
	};

	class Colossus : public Enemy
	{
	public:
		CircleShape circleDraw = CircleShape(30.0f);
		CircleShape circleDrawOrb = CircleShape(5.0f);
		CircleShape circleShadow = CircleShape(30.0f);

		Colossus(Vector2f pos) : Enemy(pos, 95.0f, 32, Color::Red)
		{
			speed = 50.f;
			mask->setRadius(30.0f);

			circleDraw.setFillColor(clrOrigin);

			circleDrawOrb.setFillColor(Color::Yellow);

			circleShadow.setFillColor(Color(Color::Black.r, Color::Black.g, Color::Black.b, Uint8(150)));
			circleShadow.setScale(Vector2f(1.0f, 0.5f));
		}
		~Colossus()
		{
			for (int i = 0; i <= random(9, 15); i++)
			{
				GutsPart* part = new GutsPart(position, direction);
				em->AddParticle(part);
			}
			//Enemy::~Enemy();
		}

		const void DoProcess() override
		{
			if (hitEffectTimer > 0) {
				hitEffectTimer = hitEffectTimer - float(1024 / 60);
				hitEffectTimer = max(hitEffectTimer, 0.f);
				float oriValue = (1.f - hitEffectTimer / hitEffectTimerMax);
				float whiValue = hitEffectTimer / hitEffectTimerMax;
				Color tColor = Color(clrOrigin.r * oriValue + Color::White.r * whiValue,
					clrOrigin.g * oriValue + Color::White.g * whiValue,
					clrOrigin.b * oriValue + Color::White.b * whiValue);
				circleDraw.setFillColor(tColor);
			}
			else { circleDraw.setFillColor(clrOrigin); }

			Vector2f tPos = em->playerEntity.position;
			Vector2f mPos = mask->getPosition();

			float dir = GetDirection(mPos, tPos);

			float togoScalra = speed / frameRatio;
			Vector2f togoVec = GetMoveDirection(dir, togoScalra);

			position += togoVec;
			maskObj.setPosition(position);
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position - Vector2f(maskObj.getRadius(), maskObj.getRadius()));

			if (hitEffectTimer > 0)
			{
				float shakeValue = maskObj.getRadius() / 5.0f * hitEffectTimer / hitEffectTimerMax;
				Vector2f shakePos = GetMoveDirection(random(0, 360), random(shakeValue, -shakeValue));
				ultPos += shakePos;
			}

			float dir = fmod(GetTime() * 0.3f, 360.f);
			Vector2f plusPos = GetMoveDirection(dir, 40.f)
				+ Vector2f(maskObj.getRadius(), maskObj.getRadius())
				- Vector2f(circleDrawOrb.getRadius(), circleDrawOrb.getRadius())
				+ Vector2f(0, 10.0f);
			DrawCircle(circleShadow, ultPos.x, ultPos.y + 40.5f, 0, 0, circleShadow.getScale(), circleShadow.getFillColor());

			if (dir < 180.f)
			{
				DrawCircle(circleDraw, ultPos.x, ultPos.y);
				DrawCircle(circleDrawOrb, ultPos.x + plusPos.x, ultPos.y + plusPos.y / 2.5f);
			}
			else {
				DrawCircle(circleDrawOrb, ultPos.x + plusPos.x, ultPos.y + plusPos.y / 2.5f);
				DrawCircle(circleDraw, ultPos.x, ultPos.y);
			}

			//nowWindow->draw(circleDraw);
		}
	};

	class Assault : public Enemy
	{
	public:
		CircleShape circleDraw = CircleShape(10.0f);
		CircleShape circleShadow = CircleShape(10.0f);
		float speedOri = 90.0f;

		Assault(Vector2f pos) : Enemy(pos, 52.0f, 23, Color(255, 80, 80))
		{
			speed = 90.f;
			speedOri = speed;
			mask->setRadius(12.0f);

			circleDraw.setFillColor(clrOrigin);

			circleShadow.setFillColor(Color(Color::Black.r, Color::Black.g, Color::Black.b, Uint8(150)));
			circleShadow.setScale(Vector2f(1.0f, 0.5f));
		}

		~Assault()
		{
			for (int i = 0; i <= random(4, 8); i++)
			{
				GutsPart* part = new GutsPart(position, direction);
				em->AddParticle(part);
			}
			//Enemy::~Enemy();
		}

		const void DoProcess() override
		{
			speed = (2.f - hpNow / hpMax) * speedOri;

			if (hitEffectTimer > 0) {
				hitEffectTimer = hitEffectTimer - float(1024 / 60);
				hitEffectTimer = max(hitEffectTimer, 0.f);
				float oriValue = (1.f - hitEffectTimer / hitEffectTimerMax);
				float whiValue = hitEffectTimer / hitEffectTimerMax;
				Color tColor = Color(clrOrigin.r * oriValue + Color::White.r * whiValue,
					clrOrigin.g * oriValue + Color::White.g * whiValue,
					clrOrigin.b * oriValue + Color::White.b * whiValue);
				circleDraw.setFillColor(tColor);
			}
			else { circleDraw.setFillColor(clrOrigin); }

			Vector2f tPos = em->playerEntity.position;
			Vector2f mPos = mask->getPosition();

			float dir = GetDirection(mPos, tPos);

			float togoScalra = speed / frameRatio;
			Vector2f togoVec = GetMoveDirection(dir, togoScalra);
			position += togoVec;
			maskObj.setPosition(position);
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position - Vector2f(maskObj.getRadius(), maskObj.getRadius()));
			if (hitEffectTimer > 0)
			{
				float shakeValue = maskObj.getRadius() / 5.0f * hitEffectTimer / hitEffectTimerMax;
				Vector2f shakePos = GetMoveDirection(random(0, 360), random(shakeValue, -shakeValue));
				ultPos += shakePos;
			}
			DrawCircle(circleShadow, ultPos.x, ultPos.y + 17.5f, 0, 0, circleShadow.getScale(), circleShadow.getFillColor());
			DrawCircle(circleDraw, ultPos.x, ultPos.y);
		}
	};

	class WarBringer : public Enemy
	{
	public:
		CircleShape circleDraw = CircleShape(38.0f);
		CircleShape circleDrawOrbFir = CircleShape(5.0f);
		CircleShape circleDrawOrbSec = CircleShape(5.0f);
		CircleShape circleShadow = CircleShape(38.0f);

		WarBringer(Vector2f pos) : Enemy(pos, 245.f, 80, Color(200, 200, 200))
		{
			speed = 70.f;

			maskObj.setRadius(38);
			circleDraw.setFillColor(clrOrigin);

			circleDrawOrbFir.setFillColor(Color(30, 30, 30));
			circleDrawOrbSec.setFillColor(Color(220, 220, 200));

			circleShadow.setFillColor(Color(Color::Black.r, Color::Black.g, Color::Black.b, Uint8(150)));
			circleShadow.setScale(Vector2f(1.0f, 0.5f));
		}
		~WarBringer()
		{
			for (int i = 0; i <= random(12, 20); i++)
			{
				GutsPart* part = new GutsPart(position, direction);
				em->AddParticle(part);
			}
			//Enemy::~Enemy();
		}

		const void DoProcess() override
		{
			if (hitEffectTimer > 0) {
				hitEffectTimer = hitEffectTimer - float(1024 / 60);
				hitEffectTimer = max(hitEffectTimer, 0.f);
				float oriValue = (1.f - hitEffectTimer / hitEffectTimerMax);
				float whiValue = hitEffectTimer / hitEffectTimerMax;
				Color tColor = Color(clrOrigin.r * oriValue + Color::White.r * whiValue,
					clrOrigin.g * oriValue + Color::White.g * whiValue,
					clrOrigin.b * oriValue + Color::White.b * whiValue);
				circleDraw.setFillColor(tColor);
			}
			else { circleDraw.setFillColor(clrOrigin); }

			Vector2f tPos = em->playerEntity.position;
			Vector2f mPos = mask->getPosition();

			float dir = GetDirection(mPos, tPos);

			float togoScalra = speed / frameRatio;
			Vector2f togoVec = GetMoveDirection(dir, togoScalra);

			position += togoVec;
			maskObj.setPosition(position);
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position - Vector2f(maskObj.getRadius(), maskObj.getRadius()));

			if (hitEffectTimer > 0)
			{
				float shakeValue = maskObj.getRadius() / 5.0f * hitEffectTimer / hitEffectTimerMax;
				Vector2f shakePos = GetMoveDirection(random(0, 360), random(shakeValue, -shakeValue));
				ultPos += shakePos;
			}

			float dir = fmod(GetTime() * 0.3f, 360.f);
			Vector2f plusPosFir = GetMoveDirection(dir, 45.f)
				+ Vector2f(maskObj.getRadius(), maskObj.getRadius())
				- Vector2f(circleDrawOrbFir.getRadius(), circleDrawOrbFir.getRadius())
				+ Vector2f(0, 10.0f)
				+ Vector2f(0, cos(dir / 360.f * 3.1415f) * 45.f);

			Vector2f plusPosSec = GetMoveDirection(dir + 180.f, 45.f)
				+ Vector2f(maskObj.getRadius(), maskObj.getRadius())
				- Vector2f(circleDrawOrbSec.getRadius(), circleDrawOrbSec.getRadius())
				+ Vector2f(0, 10.0f)
				+ Vector2f(0, sin(dir / 360.f * 3.1415f) * 45.f);
			DrawCircle(circleShadow, ultPos.x, ultPos.y + 40.5f, 0, 0, circleShadow.getScale(), circleShadow.getFillColor());

			if (dir < 180.f)
			{
				DrawCircle(circleDrawOrbSec, ultPos.x + plusPosSec.x, ultPos.y + plusPosSec.y / 2.5f);
				DrawCircle(circleDraw, ultPos.x, ultPos.y);
				DrawCircle(circleDrawOrbFir, ultPos.x + plusPosFir.x, ultPos.y + plusPosFir.y / 2.5f);
			}
			else {
				DrawCircle(circleDrawOrbFir, ultPos.x + plusPosFir.x, ultPos.y + plusPosFir.y / 2.5f);
				DrawCircle(circleDraw, ultPos.x, ultPos.y);
				DrawCircle(circleDrawOrbSec, ultPos.x + plusPosSec.x, ultPos.y + plusPosSec.y / 2.5f);
			}

			//nowWindow->draw(circleDraw);
		}
	};

	class DarkOne : public Enemy
	{
	public:
		CircleShape circleDraw = CircleShape(12.0f);
		CircleShape circleDrawOrb = CircleShape(3.0f);
		CircleShape circleShadow = CircleShape(12.0f);

		DarkOne(Vector2f pos) :Enemy(pos, 88.0f, 45, Color(65, 65, 65))
		{
			speed = 165.0f;
			mask->setRadius(12.0f);

			circleDraw.setFillColor(clrOrigin);
			circleDrawOrb.setFillColor(Color(220, 220, 220));
			circleShadow.setFillColor(Color(Color::Black.r, Color::Black.g, Color::Black.b, Uint8(150)));
			circleShadow.setScale(Vector2f(1.0f, 0.5f));

			circleDraw.setOrigin(circleDraw.getRadius(), circleDraw.getRadius());
			circleDrawOrb.setOrigin(circleDrawOrb.getRadius(), circleDrawOrb.getRadius());
			circleShadow.setOrigin(circleShadow.getRadius(), circleShadow.getRadius());
			maskObj.setOrigin(maskObj.getRadius(), maskObj.getRadius());
		}
		~DarkOne()
		{
			for (int i = 0; i <= random(4, 8); i++)
			{
				GutsPart* part = new GutsPart(position, direction);
				em->AddParticle(part);
			}
			//Enemy::~Enemy();
		}

		const void DoProcess() override
		{

			if (hitEffectTimer > 0) {
				hitEffectTimer = hitEffectTimer - float(1024 / 60);
				hitEffectTimer = max(hitEffectTimer, 0.f);
				float oriValue = (1.f - hitEffectTimer / hitEffectTimerMax);
				float whiValue = hitEffectTimer / hitEffectTimerMax;
				Color tColor = Color(clrOrigin.r * oriValue + Color::White.r * whiValue,
					clrOrigin.g * oriValue + Color::White.g * whiValue,
					clrOrigin.b * oriValue + Color::White.b * whiValue);
				circleDraw.setFillColor(tColor);
			}
			else { circleDraw.setFillColor(clrOrigin); }

			Vector2f tPos = em->playerEntity.position;
			Vector2f mPos = mask->getPosition();

			float dir = GetDirection(mPos, tPos);

			float togoScalra = speed / frameRatio;
			Vector2f togoVec = GetMoveDirection(dir, togoScalra);
			position += togoVec;
			maskObj.setPosition(position);
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position);
			if (hitEffectTimer > 0)
			{
				float shakeValue = maskObj.getRadius() / 5.0f * hitEffectTimer / hitEffectTimerMax;
				Vector2f shakePos = GetMoveDirection(random(0, 360), random(shakeValue, -shakeValue));
				ultPos += shakePos;
			}


			DrawCircle(circleShadow, ultPos.x, ultPos.y + 17.5f, 0, 0, circleShadow.getScale(), circleShadow.getFillColor());
			DrawCircle(circleDraw, ultPos.x, ultPos.y);
			Vector2f plusPosFir = GetMoveDirection(random(0.f, 360.f), random(0.f, 2.f)) + GetMoveDirection(GetDirection(position, em->playerEntity.position), 1.f);
			ultPos += plusPosFir;
			DrawCircle(circleDrawOrb, ultPos.x + plusPosFir.x, ultPos.y + plusPosFir.y);

		}
	};

	class Untalkable : public Enemy
	{
	public:
		CircleShape circleDraw = CircleShape(50.0f);
		CircleShape circleDrawOrb = CircleShape(20.0f);
		CircleShape circleShadow = CircleShape(50.0f);

		Untalkable(Vector2f pos) : Enemy(pos, 600.0f, 10, Color(65, 65, 65))
		{
			speed = 170.0f;

			maskObj.setRadius(50.0f);
			circleDraw.setFillColor(clrOrigin);

			circleDrawOrb.setFillColor(Color(220, 220, 220));

			circleShadow.setFillColor(Color(Color::Black.r, Color::Black.g, Color::Black.b, Uint8(150)));
			circleShadow.setScale(Vector2f(1.0f, 0.5f));

			maskObj.setOrigin(maskObj.getRadius(), maskObj.getRadius());
			circleDraw.setOrigin(circleDraw.getRadius(), circleDraw.getRadius());
			circleShadow.setOrigin(circleShadow.getRadius(), circleShadow.getRadius());
			circleDrawOrb.setOrigin(circleDrawOrb.getRadius(), circleDrawOrb.getRadius());
		}
		~Untalkable()
		{
			for (int i = 0; i <= random(12, 20); i++)
			{
				GutsPart* part = new GutsPart(position, direction);
				em->AddParticle(part);
			}
		}

		const void DoProcess() override
		{
			if (hitEffectTimer > 0) {
				hitEffectTimer = hitEffectTimer - float(1024 / 60);
				hitEffectTimer = max(hitEffectTimer, 0.f);
				float oriValue = (1.f - hitEffectTimer / hitEffectTimerMax);
				float whiValue = hitEffectTimer / hitEffectTimerMax;
				Color tColor = Color(clrOrigin.r * oriValue + Color::White.r * whiValue,
					clrOrigin.g * oriValue + Color::White.g * whiValue,
					clrOrigin.b * oriValue + Color::White.b * whiValue);
				circleDraw.setFillColor(tColor);
			}
			else { circleDraw.setFillColor(clrOrigin); }

			Vector2f tPos = em->playerEntity.position;
			Vector2f mPos = mask->getPosition();

			float dir = GetDirection(mPos, tPos);

			float togoScalra = speed / frameRatio;
			Vector2f togoVec = GetMoveDirection(dir, togoScalra);

			position += togoVec;
			maskObj.setPosition(position);
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position);

			if (hitEffectTimer > 0)
			{
				float shakeValue = maskObj.getRadius() / 5.0f * hitEffectTimer / hitEffectTimerMax;
				Vector2f shakePos = GetMoveDirection(random(0, 360), random(shakeValue, -shakeValue));
				ultPos += shakePos;
			}

			Vector2f plusPosFir = GetMoveDirection(random(0.f, 360.f), random(0.f, 5.f)) + GetMoveDirection(GetDirection(position, em->playerEntity.position), 10.f);

			DrawCircle(circleShadow, ultPos.x, ultPos.y + circleDraw.getRadius(), 0, 0, circleShadow.getScale(), circleShadow.getFillColor());
			DrawCircle(circleDraw, ultPos.x, ultPos.y);
			ultPos += plusPosFir;
			DrawCircle(circleDrawOrb, ultPos.x + plusPosFir.x, ultPos.y + plusPosFir.y);

		}
	};


	//�÷��̾� ����ü
	class HandGunBullet : public Projectile
	{
	public:
		HandGunBullet(Vector2f pos, float dir, int lv) : Projectile(pos, dir, lv)
		{
			maskObj = CircleShape(4.0f);
			maskObj.setFillColor(Color::Yellow);
			maskObj.setPosition(pos);
			maskObj.setOrigin(maskObj.getRadius(), maskObj.getRadius());
			maskObj.setScale(2.f, 1.f);
			position = pos;
			isFriendly = true;

			damage = lv >= 10 ? 52.0f : 13.f + lv * 1.1f;
			speed = lv >= 10 ? 1000.f : 500.0f;
			hitCount = 1 + lv;
		}
		~HandGunBullet()
		{
			for (int i = 0; i <= random(6, 12); i++)
			{
				BulletPart* part = new BulletPart(position, direction);
				em->AddParticle(part);
			}
		}
		Vector2f position;
		CircleShape maskObj = CircleShape(6.0f);

		float speed = 500.0f;
		int hitCount = 2;
		float damage = 13.0f;
		int lifeTime = 200.0f;

		bool isFriendly = true;
		list<Enemy*> hittedList;

		void LifeRefresh() override { lifeTime--; }
		bool IsExpired() override { return lifeTime < 0 || hitCount == 0 ? true : false; }

		const void DoProcess() override
		{
			isFriendly = true;
			float togoScalra = speed / frameRatio;
			Vector2f togoVec = GetMoveDirection(direction, togoScalra);
			position += togoVec;
			maskObj.setPosition(position);
			maskObj.setRotation(direction);
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position);

			DrawCircle(maskObj, ultPos.x, ultPos.y);
		}
		const void HitProcess(Enemy* enemyT) override
		{
			if (hitCount == 0) { return; }
			for (Enemy* enemy : hittedList) {	//��밡 �ǰݵ� �� ���ٸ�
				if (enemy == enemyT) {
					return;
				}
			}
			hittedList.push_back(enemyT);	//�ǰ��� ������ ����Ѵ�.
			enemyT->hpNow -= damage;	//���ظ� �ش�.
			enemyT->SetHit();
			hitCount = hitCount == -1 ? -1 : hitCount - 1;
		} 	//������ ����ũ ���߽�
		const void EffectProcess(Enemy* enemy) override {}	//������ ȿ�� �����
		const bool DoCheckCollision(Enemy* enemy) override
		{
			return CheckCollision(maskObj, *(enemy->mask));
		}

	};

	class ShotGunBullet : public Projectile
	{
	public:
		ShotGunBullet(Vector2f pos, float dir, int lv) : Projectile(pos, dir, lv)
		{
			maskObj = CircleShape(2.4f);
			maskObj.setFillColor(Color((Color::Yellow.r + Color::Red.r) / 2, (Color::Yellow.g + Color::Red.g) / 2, (Color::Yellow.b + Color::Red.b) / 2));
			maskObj.setPosition(pos);
			position = pos;
			isFriendly = true;

			damage = 9.0f + lv * 0.6f;
			if (lv >= 10)
			{
				speed = 450.f;
				lifeTime = 60.0f;
			}
		}
		~ShotGunBullet()
		{
			for (int i = 0; i <= random(2, 3); i++)
			{
				BulletPart* part = new BulletPart(position, direction);
				em->AddParticle(part);
				part->life /= 2;
			}
		}
		Vector2f position;
		CircleShape maskObj = CircleShape(3.0f);

		float speed = 340.0f;
		int hitCount = 1;
		float damage = 9.0f;
		int lifeTime = 35.0f;

		bool isFriendly = true;
		list<Enemy*> hittedList;

		void LifeRefresh() override { lifeTime--; }
		bool IsExpired() override { return lifeTime < 0 || hitCount == 0 ? true : false; }

		const void DoProcess() override
		{
			isFriendly = true;
			float togoScalra = speed / frameRatio;
			Vector2f togoVec = GetMoveDirection(direction, togoScalra);
			position += togoVec;
			maskObj.setPosition(position);
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position);

			DrawCircle(maskObj, ultPos.x, ultPos.y);
		}
		const void HitProcess(Enemy* enemyT) override
		{
			if (hitCount == 0) { return; }
			for (Enemy* enemy : hittedList) {	//��밡 �ǰݵ� �� ���ٸ�
				if (enemy == enemyT) {
					return;
				}
			}
			hittedList.push_back(enemyT);	//�ǰ��� ������ ����Ѵ�.
			enemyT->hpNow -= damage;	//���ظ� �ش�.
			enemyT->SetHit();
			hitCount = hitCount == -1 ? -1 : hitCount - 1;
		} 	//������ ����ũ ���߽�
		const void EffectProcess(Enemy* enemy) override {}	//������ ȿ�� �����
		const bool DoCheckCollision(Enemy* enemy) override
		{
			return CheckCollision(maskObj, *(enemy->mask));
		}

	};

	class SmgBullet : public Projectile
	{
	public:
		SmgBullet(Vector2f pos, float dir, int lv) : Projectile(pos, dir, lv)
		{
			maskObj = CircleShape(3.f);
			maskObj.setFillColor(Color(Color::Yellow.r * 0.7f, Color::Yellow.g * 0.7f, Color::Yellow.b * 0.7f));
			maskObj.setPosition(pos);
			position = pos;
			isFriendly = true;

			damage = 5.0f + lv * .4f;
		}
		~SmgBullet()
		{
			for (int i = 0; i <= random(2, 4); i++)
			{
				BulletPart* part = new BulletPart(position, direction);
				em->AddParticle(part);
			}
		}
		Vector2f position;
		CircleShape maskObj;

		float speed = 600.0f;
		int hitCount = 1;
		float damage = 5.0f;
		int lifeTime = 30.0f;

		bool isFriendly = true;
		list<Enemy*> hittedList;

		void LifeRefresh() override { lifeTime--; }
		bool IsExpired() override { return lifeTime < 0 || hitCount == 0 ? true : false; }

		const void DoProcess() override
		{
			isFriendly = true;
			float togoScalra = speed / frameRatio;
			Vector2f togoVec = GetMoveDirection(direction, togoScalra);
			position += togoVec;
			maskObj.setPosition(position);
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position);

			DrawCircle(maskObj, ultPos.x, ultPos.y);
		}
		const void HitProcess(Enemy* enemyT) override
		{
			if (hitCount == 0) { return; }
			for (Enemy* enemy : hittedList) {	//��밡 �ǰݵ� �� ���ٸ�
				if (enemy == enemyT) {
					return;
				}
			}
			hittedList.push_back(enemyT);	//�ǰ��� ������ ����Ѵ�.
			enemyT->hpNow -= damage;	//���ظ� �ش�.
			enemyT->SetHit();
			hitCount = hitCount == -1 ? -1 : hitCount - 1;
		} 	//������ ����ũ ���߽�
		const void EffectProcess(Enemy* enemy) override {}	//������ ȿ�� �����
		const bool DoCheckCollision(Enemy* enemy) override
		{
			return CheckCollision(maskObj, *(enemy->mask));
		}

	};

	class RailGunRay : public Projectile
	{
	public:
		RailGunRay(Vector2f pos, float dir, int lv) : Projectile(pos, dir, lv)
		{
			position = pos;

			drawCircle = CircleShape(20.0f);
			drawCircle.setFillColor(Color(120, 120, 255));
			drawCircle.setPosition(pos);

			maskObj = RectangleShape(Vector2f(1500.f, 20.f));
			maskObj.setFillColor(Color(120, 120, 255));
			maskObj.setRotation(dir);
			maskObj.setPosition(pos);
			isFriendly = true;

			laserWide = 20.f + lv * 1.0f;
			damage = 32.0f + lv * 2.8f;

		}
		~RailGunRay() {}

		Vector2f position;
		RectangleShape maskObj;
		CircleShape drawCircle;

		int hitCount = -1;
		float damage = 32.0f;
		const int lifeTimeMax = 25;
		int lifeTime = 25;

		float laserWide = 20.0f;

		bool isFriendly = true;
		list<Enemy*> hittedList;

		void LifeRefresh() override { lifeTime--; }
		bool IsExpired() override { return lifeTime < 0 || hitCount == 0 ? true : false; }

		const void DoProcess() override
		{
			drawCircle.setRadius(laserWide * lifeTime / lifeTimeMax);
			drawCircle.setOrigin(Vector2f(drawCircle.getRadius(), drawCircle.getRadius()));
			maskObj.setSize(Vector2f(1500.f, laserWide * lifeTime / lifeTimeMax));
			maskObj.setOrigin(Vector2f(0, maskObj.getSize().y / 2));

			float lifeRatio = (float)lifeTime / (float)lifeTimeMax;

			maskObj.setFillColor(Color(120 + 135 * lifeRatio, 120 + 135 * lifeRatio, 255));
			drawCircle.setFillColor(Color(120 + 135 * lifeRatio, 120 + 135 * lifeRatio, 255));

			for (int i = 0; i <= random(0, 2 * lifeTime / lifeTimeMax); i++)
			{
				RayPart* part = new RayPart(position, random(0.f, 360.f));
				em->AddParticle(part);
			}
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position);
			maskObj.setPosition(ultPos);
			drawCircle.setPosition(ultPos);
			nowWindow->draw(maskObj);
			nowWindow->draw(drawCircle);
			maskObj.setPosition(position);
		}
		const void HitProcess(Enemy* enemyT) override
		{
			if (lifeTime >= lifeTimeMax - 1) { return; }
			if (hitCount == 0) { return; }
			for (Enemy* enemy : hittedList) {	//��밡 �ǰݵ� �� ���ٸ�
				if (enemy == enemyT) {
					return;
				}
			}
			hittedList.push_back(enemyT);	//�ǰ��� ������ ����Ѵ�.
			enemyT->hpNow -= damage;	//���ظ� �ش�.
			enemyT->SetHit();

			for (int i = 0; i <= random(10, 15); i++)
			{
				RayPart* part = new RayPart(enemyT->position, direction + random(-30.f, 30.f));
				em->AddParticle(part);
			}

			hitCount = hitCount == -1 ? -1 : hitCount - 1;
		} 	//������ ����ũ ���߽�
		const void EffectProcess(Enemy* enemy) override {}	//������ ȿ�� �����
		const bool DoCheckCollision(Enemy* enemy) override
		{
			return CheckCollision(maskObj, *(enemy->mask));
		}

	};

	class SpiralMissile : public Projectile
	{
	public:
		SpiralMissile(Vector2f pos, float dir, int lv) : Projectile(pos, dir, lv)
		{
			maskObj = CircleShape(6.0f);
			maskObj.setFillColor(Color::Red);
			maskObj.setPosition(pos);
			maskObj.setOrigin(Vector2f(maskObj.getRadius(), maskObj.getRadius()));
			position = pos;
			lifeTime = lifeTimeMax;
			angleSpeed = 0;
			speed = 0;

			damage = 30.f + lv * 2.4f;
			explosionRangeMax = 40.f + lv * 3.f;
		}
		~SpiralMissile()
		{
		}

		Vector2f position;
		CircleShape maskObj;

		float speedMax = 300.0f;
		float accel = 280.0f;

		int hitCount = -1;
		float damage = 30.0f;
		int lifeTime, lifeTimeMax = 500;

		bool isFriendly = true;
		list<Enemy*> hittedList;

		float angleSpeed, angleSpeedMax = 2.0f, angleAccel = 20.0f;
		Enemy* target = nullptr;

		bool isExplosed = false, isExplosedStart = true;
		int explosionTime = 0, explosionTimeMax = 60;
		float explosionRange = 6.f, explosionRangeMax = 40.f;

		void LifeRefresh() override { lifeTime--; }
		bool IsExpired() override { return !isExplosed && (lifeTime < 0) || hitCount == 0 ? true : false; }

		const void DoProcess() override
		{
			if (isExplosed)	//���� ��
			{
				if (isExplosedStart)
				{
					isExplosedStart = false;
					sm->PlaySound("mlboom");
				}

				float alphaValue = 1.f - (float)explosionTime / explosionTimeMax;
				explosionRange = pow((float)explosionTime / explosionTimeMax, 0.5f) * explosionRangeMax;


				maskObj.setRadius(explosionRange);

				if (explosionTimeMax * 0.9f <= (float)explosionTime && explosionTime < explosionTimeMax)
				{//���� ������

					float value = 1.f - ((float)explosionTime - explosionTimeMax * 0.9f) / explosionTimeMax * 0.1f;//1 >> 0
					//alphaValue = value;
					maskObj.setRadius(explosionRange * value);
				}
				else if (explosionTime >= explosionTimeMax)
				{//���� ����
					isExplosed = false;
					lifeTime = 0;
				}

				maskObj.setFillColor(Color(255, 255, 255, Uint8(255 * alphaValue)));
				maskObj.setOrigin(Vector2f(maskObj.getRadius(), maskObj.getRadius()));

				explosionTime++;

			}
			else { //������

				//���� ó��
				if (target == nullptr)	//��ǥ ����
				{
					angleSpeed += random(angleAccel / frameRatio, -angleAccel / frameRatio);
					target = lifeTimeMax - 30.0f > lifeTime ? em->GetClosestEnemy(position + Vector2f(random(250.f, -250.f), random(250.f, -250.f))) : nullptr;
				}
				else { //��ǥ ����
					angleSpeed = isClockwiseClose(direction, GetDirection(position, target->position)) ?
						angleSpeed + angleAccel / frameRatio : angleSpeed = angleSpeed - angleAccel / frameRatio;
					if (GetDistance(position, target->position) < explosionRangeMax * 0.6f)
					{
						isExplosed = true;
					}
					target = target->IsExpired() ? nullptr : target;

				}

				angleSpeed = angleSpeed > angleSpeedMax ? angleSpeedMax : angleSpeed;
				angleSpeed = angleSpeed < -angleSpeedMax ? -angleSpeedMax : angleSpeed;



				//�̵� ó��
				speed = min(speed + accel / frameRatio, speedMax);
				float togoScalra = speed / frameRatio;
				Vector2f togoVec = GetMoveDirection(direction, togoScalra);
				position += togoVec;
				maskObj.setPosition(position);

				direction += angleSpeed;

				//����� ����
				if (lifeTime <= 1) { isExplosed = true; }

				//��ƼŬ ����
				MissileTailPart* part = new MissileTailPart(position, random(0.f, 360.f));
				em->AddParticle(part);
			}
		}
		const void DrawProcess() override
		{
			Vector2f ultPos = cm->TranslatePosition(position);
			maskObj.setRotation(direction);
			maskObj.setScale(2.f - isExplosed, 1.f);
			maskObj.setPosition(ultPos);
			nowWindow->draw(maskObj);
			maskObj.setPosition(position);

			if (!isExplosed)
			{
				DrawLine(ultPos, ultPos - GetMoveDirection(direction, 12.f), maskObj.getRadius() * 2.f, Color(210, 210, 210));
			}
		}
		const void HitProcess(Enemy* enemyT) override
		{
			if (hitCount == 0) { return; }
			for (Enemy* enemy : hittedList) {	//��밡 �ǰݵ� �� ���ٸ�
				if (enemy == enemyT) {
					return;
				}
			}
			hittedList.push_back(enemyT);	//�ǰ��� ������ ����Ѵ�.
			enemyT->hpNow -= damage;	//���ظ� �ش�.
			enemyT->SetHit();
			isExplosed = true;
			hitCount = hitCount == -1 ? -1 : hitCount - 1;
		} 	//������ ����ũ ���߽�
		const void EffectProcess(Enemy* enemy) override {}	//������ ȿ�� �����
		const bool DoCheckCollision(Enemy* enemy) override
		{
			return CheckCollision(maskObj, *(enemy->mask));
		}

	};
#pragma endregion

#pragma region [���� : ����]
	void HandGun::DoFire()
	{
		if (em->enemyEntityList.size() == 0)
		{
			timerNow = 0;
			return;
		}	//������ ���� ���ٸ� �������� ����.

		Vector2f startPoint = em->playerEntity.position;
		HandGunBullet* proj = new HandGunBullet(startPoint, GetDirection(startPoint, em->GetClosestEnemy(startPoint)->position) + random(2.0f, -2.0f), level);
		//�÷��̾��� ��ġ���� ���� ����� ���� ���� ź�� ����.
		em->AddProjectile(proj);

		sm->PlaySound("hgFire");
	}

	void ShotGun::DoFire()
	{
		timeFromFire = 0;

		if (em->enemyEntityList.size() == 0)
		{
			timerNow = 0;
			return;
		}	//������ ���� ���ٸ� �������� ����.

		Vector2f startPoint = em->playerEntity.position;
		float dir = GetDirection(startPoint, em->GetClosestEnemy(startPoint)->position);
		for (int i = -3 - level / 2; i <= 3 + level / 2; i++) {
			ShotGunBullet* proj = new ShotGunBullet(startPoint, dir + (float)i * 5.f, level);
			//�÷��̾��� ��ġ���� ���� ����� ���� ���� ź�� ����.
			em->AddProjectile(proj);
		}

		sm->PlaySound("sgFire");
	}
	const void ShotGun::DoProcessAttach() {
		timeFromFire++;
		if (timeFromFire == 20 && level >= 10) {
			DoFire();
			timeFromFire = 21;
		}
	}

	void SubMachineGun::DoFire()
	{
		if (magNow == magMax)
		{
			isReloading = false;
			timerMax = timerMaxFire;
		}
		if (isReloading)
		{
			timerMax = timerMaxReload;
			magNow++;
			return;
		}
		if (magNow == 0)
		{
			isReloading = true;
			return;
		}
		//reload mechanism

		if (em->enemyEntityList.size() == 0) {
			timerNow = 0;
			isReloading = magNow == magMax ? false : true;
			return;
		}	//������ ���� ���ٸ� �������� ����.

		sm->PlaySound("smgFire");
		magNow--;
		Vector2f startPoint = em->playerEntity.position;
		SmgBullet* proj = new SmgBullet(startPoint, GetDirection(startPoint, em->GetClosestEnemy(startPoint)->position) + random(10.0f, -10.0f), level);
		//�÷��̾��� ��ġ���� ���� ����� ���� ���� ź�� ����.
		em->AddProjectile(proj);

		if (level >= 10)
		{
			Vector2f startPoint = em->playerEntity.position;
			SmgBullet* proj = new SmgBullet(startPoint, GetDirection(startPoint, em->GetClosestEnemy(startPoint)->position) + random(10.0f, -10.0f), level);
			//�÷��̾��� ��ġ���� ���� ����� ���� ���� ź�� ����.
			em->AddProjectile(proj);
		}

	}

	void RailGun::DoFire()
	{
		if (em->enemyEntityList.size() == 0) {
			timerNow = 0;
			return;
		}	//������ ���� ���ٸ� �������� ����.
		fireCountNow = fireCountMax;
		fireTimeNow = fireTimeMax;

		Vector2f startPoint = em->playerEntity.position;
		direction = GetDirection(startPoint, em->GetClosestEnemy(startPoint)->position);

		sm->PlaySound("rgFire");
	}
	const void RailGun::DoProcessAttach()
	{
		fireTimeNow--;

		if (fireTimeNow == 0 && fireCountNow != 0) {

			fireTimeNow = fireTimeMax;
			fireCountNow--;

			Vector2f startPoint = em->playerEntity.position;
			RailGunRay* proj = new RailGunRay(startPoint, direction, level);
			//�÷��̾��� ��ġ���� ���� ����� ���� ���� ź�� ����.
			em->AddProjectile(proj);
		}
	}
	const void RailGun::DrawProcess()
	{
		if (fireTimeNow > 0 && fireCountNow != 0)
		{
			Vector2f startPos = em->playerEntity.position;
			startPos = cm->TranslatePosition(startPos);
			Color color = Color(Color::Red.r * sin(GetTime() / 100.0f), Color::Black.g, Color::Black.b);
			DrawLine(startPos, startPos + GetMoveDirection(direction, 2000.f), 1.2f, color);
		}
	}

	void SpiralMissileLauncher::DoFire()
	{
		if (em->enemyEntityList.size() == 0) {
			timerNow = 0;
			return;
		}	//������ ���� ���ٸ� �������� ����.

		fireCountNow = fireCountMax;
		fireTimeNow = fireTimeMax;

		Vector2f startPoint = em->playerEntity.position;
		direction = GetDirection(startPoint, em->GetClosestEnemy(startPoint)->position);
	}
	const void SpiralMissileLauncher::DoProcessAttach()
	{
		fireTimeNow--;

		if (fireTimeNow == 0 && fireCountNow != 0) {

			fireTimeNow = fireTimeMax;
			fireCountNow--;

			direction += level <= 10 ? random(10.f, -10.f) : random(30.f, -30.f);

			Vector2f startPoint = em->playerEntity.position;
			SpiralMissile* proj = new SpiralMissile(startPoint, direction, level);
			//�÷��̾��� ��ġ���� ���� ����� ���� ���� ����ü ����.
			em->AddProjectile(proj);

			sm->PlaySound("mlFire");
		}
	}

#pragma endregion

#pragma region [�ý��� Ŭ���� : ����]
	void InputManager::TestFunctionF1()
	{
		lm->LevelUp();

	}
	void InputManager::TestFunctionF2()
	{
		wm->timeStart -= (long long)1024 * 60;
	}
	void InputManager::TestFunctionF3()
	{
		//����� ��� ����
		for (int i = 0; i <= wm->phaseMax - 1; i++)
		{
			wm->phaseTimeArr[i] = 60 * (i + 1);
		}

	}
	void WorldManager::SpawnProcess()
	{
		spawnValue += (float)phaseSwarmDeltaArr[phase] / frameRatio;

		if (em->enemyEntityList.size() > phaseSwarmMaxArr[phase]) { return; }
		if (spawnValue < 1.0f) { return; }
		spawnValue--;
		SpawnEnemyByPhase();

	}
	void WorldManager::SpawnEnemyByPhase()
	{
		EnemyCode choosenCode = ERROR;

		//����� ���� �ڵ� Ȯ��
		switch (phase)
		{
		case 1:
			if (random(0.f, 12.0f) < 1.f) { choosenCode = COLOSSUS; }
			else { choosenCode = SWARMER; }
			break;
		case 2:
			if (random(0.f, 9.0f) < 1.f) { choosenCode = COLOSSUS; }
			//if (random(0.f, 30.0f) < 1.f) { choosenCode = ILLUSION; }
			else { choosenCode = SWARMER; }
			break;
		case 3:
			if (random(0.f, 7.0f) < 1.f) { choosenCode = COLOSSUS; }
			//if (random(0.f, 30.0f) < 1.f) { choosenCode = ILLUSION; }
			//if (random(0.f, 40.0f) < 1.f) { choosenCode = PIERCER; }
			else { choosenCode = ASSAULT; }
			break;
		case 4:
			if (random(0.f, 12.0f) < 1.f) { choosenCode = WARBRINGER; }
			//if (random(0.f, 25.0f) < 1.f) { choosenCode = PIERCER; }
			if (random(0.f, 8.0f) < 1.f) { choosenCode = DARKONE; }
			else { choosenCode = ASSAULT; }
			break;
		case 5:
			if (random(0.f, 8.5f) < 1.f) { choosenCode = WARBRINGER; }
			//if (random(0.f, 20.0f) < 1.f) { choosenCode = HYBRID; }
			else { choosenCode = DARKONE; }
			break;
		case 6:
			choosenCode = UNTALKABLE;
			break;
		default:
			um->SetDebugMessage("[SpawnEnemyByPhase()]���ǵ��� ���� �ڵ�! �� ���� ����!");
		}

		//���̺긮�� ���� ó��
		if (hybridCount != 0)
		{
			//choosenCode = HYBRID;
		}

		//��ġ ���
		Vector2f position = GetMoveDirection(random(0.f, 360.f), 1400.0f) + em->playerEntity.position;

		//��ü�� ���� ������ ����
		Enemy* enm = nullptr;

		//�ڵ忡 ���� ��ü ����
		switch (choosenCode)
		{
		case SWARMER:
			enm = new Swarmer(position);
			break;
		case COLOSSUS:
			enm = new Colossus(position);
			break;
		case ILLUSION:
			//
			break;
		case ASSAULT:
			enm = new Assault(position);
			break;
		case PIERCER:
			//
			break;
		case WARBRINGER:
			enm = new WarBringer(position);
			break;
		case DARKONE:
			enm = new DarkOne(position);
			break;
		case HYBRID:
			//
			break;
		case UNTALKABLE:
			enm = new Untalkable(position);
			break;
		case ERROR:
			return;
		}

		if (enm == nullptr) { return; }

		//������ ���� em�� �߰�
		em->AddEnemy(enm);
	}
	void UiManager::UiProcess()
	{
		string tMin = to_string((GetTime() / (long long)60000));
		string tSec = to_string((GetTime() / (long long)1000) % 60);
		string tMil = to_string((GetTime() / (long long)10) % 100);

		tMin = (GetTime() / (long long)60000) <= 9 ? "0" + tMin : tMin;
		tSec = (GetTime() / (long long)1000) % 60 <= 9 ? "0" + tSec : tSec;
		tMil = (GetTime() / (long long)10) % 100 <= 9 ? "0" + tMil : tMil;

		textLevel.setString(to_string(lm->playerLevel));
		textTimer.setString(tMin + ":" + tSec + "." + tMil);
		textExp.setString("exp : " + to_string(lm->stackedExp) + "(" + to_string(lm->nowExp) + "/" + to_string(lm->requireExp) + ")");
		textHp.setString("HP : " + to_string(em->playerEntity.hp) + " / 5");


		barExpValue = (barExpValue * 9.f + lm->expRatio) / 10.f;
		//SetDebugMessage(to_string(barExpValue);
		barExp.setFillColor(
			Color(
				Uint8(255 * min(barExpValue, 1.f)),
				Uint8(255 * min(pow(barExpValue - 1.f, 2.f), 1.f)),
				Uint8(255 * min(1.f, 1.f - barExpValue))));
		barExp.setSize(Vector2f(screenSize.x * barExpValue, barExp.getSize().y));

		if (em->playerEntity.hp <= 0 && !isGameEnded) {
			textEndTime.setString(sf::String(L"����� �ð� : " + textTimer.getString()));
			textScore.setString(sf::String(L"���� ���� : " + to_string(lm->stackedExp)));
			isGameEnded = true;
		}

	}
	void EntityManager::CheckProjectile(auto& projectile, bool isFriendly)
	{
		for (const auto& enemy : enemyEntityList) {
			bool collisionResult = projectile.DoCheckCollision(enemy.get());

			if (collisionResult)
			{
				sm->PlaySound("enemyHit");
				projectile.HitProcess(enemy.get());
			}
		}
	}

#pragma endregion

#pragma region [�⺻ Ŭ���� : ����]
	Player::Player(Vector2f pos) :position(pos) {
		invincibleTime--;

		maskObj.setFillColor(Color::White);

		circleShadow.setFillColor(Color(Color::Black.r, Color::Black.g, Color::Black.b, Uint8(150)));
		circleShadow.setScale(Vector2f(1.0f, 0.5f));

	}
	void Player::DoProcess()
	{
		//�̵� ó��
		Vector2i moveInput = Vector2i(im->rightPressed - im->leftPressed, im->downPressed - im->upPressed);
		if (moveInput.x != 0 || moveInput.y != 0)
		{
			direction = GetDirection(Vector2f(0, 0), Vector2f(moveInput));
			Vector2f toMove = GetMoveDirection(direction, speed / frameRatio);
			position += toMove;
		}

		invincibleTime--;
		for (const auto& enemy : em->enemyEntityList)
		{
			if (invincibleTime > 0) { return; }

			if (CheckCollision(maskObj, enemy.get()->maskObj))
			{
				GetDamage();
				return;
			}
		}
	}
	void Player::DrawProcess()
	{
		Vector2f ultPos = cm->TranslatePosition(position - Vector2f(maskObj.getRadius(), maskObj.getRadius()));

		Color color = invincibleTime >= 0 ? Color(255, 255, 255, Uint8(255 * sin(GetTime() / 1024.f * 10.0f))) : Color::White;

		maskObj.setFillColor(color);

		DrawCircle(circleShadow, ultPos.x, ultPos.y + 17.5f, 0, 0, circleShadow.getScale(), circleShadow.getFillColor());
		DrawCircle(maskObj, ultPos.x, ultPos.y);
		maskObj.setPosition(position);
	}
	void Player::GetDamage()
	{
		sm->PlaySound("playerHit");
		timeToProcess -= 100.f;
		invincibleTime = invincibleTimeMax;
		hp--;
	}

	Enemy::Enemy(Vector2f pos, float hp, int expPoint, Color clrOri) : position(pos), hpMax(hp), hpNow(hp), exp(expPoint), clrOrigin(clrOri) {}
	Enemy::~Enemy() {

		sm->PlaySound("enemyDie");
		lm->GetExp(exp);
	}
	const void Enemy::ResetFarPosition()
	{
		if (GetDistance(position, em->playerEntity.position) <= 1600.0f) { return; }

		Vector2f newPosition = GetMoveDirection(random(0.f, 360.f), 1400.0f) + em->playerEntity.position;
		position = newPosition;
		hpNow = hpMax;
	}
	const void Enemy::SetHit()
	{
		sm->PlaySound("enemyHit");
		hitEffectTimer = hitEffectTimerMax;
	}

	Projectile::Projectile(Vector2f pos, float dir, int lv) : direction(dir), level(lv) {}

	const void Projectile::CollisionProcess()
	{
		if (true) {
			for (const auto& enemyT : em->enemyEntityList)
			{
				if (DoCheckCollision(enemyT.get()))
				{
					HitProcess(enemyT.get());
				}
			}
		}
		else
		{
			//CheckCollision(mask)
		}
	}

	void Weapon::DoChooserProcess()
	{
		float speed = 40.f;
		chooseValue = (chooseValue * speed + float(lm->isChoosing)) / (speed + 1.f);

		if (lm->isChoosing == false)
		{
			centerTarget = Vector2f(screenSize.x / 2.f, 5000.0f);
		}


		float centerSpeed = 10.f;
		centerDraw = Vector2f((centerDraw.x * centerSpeed + centerTarget.x) / (centerSpeed + 1.f),
			(centerDraw.y * centerSpeed + centerTarget.y) / (centerSpeed + 1.f));

		boxBorder.setPosition(centerDraw);
		sprChooserIcon.setPosition(centerDraw + Vector2f(0.f, -200.f));	//y ���� -200

		txtLevel.setPosition(centerDraw + Vector2f(-50.f, -100.f));
		txtName.setPosition(centerDraw + Vector2f(-170.f, -50.f));
		txtDescript.setPosition(centerDraw + Vector2f(-170.f, -0.f));
		txtPressButton.setPosition(centerDraw + Vector2f(-170.f, +100.f));
	}


#pragma endregion
}

