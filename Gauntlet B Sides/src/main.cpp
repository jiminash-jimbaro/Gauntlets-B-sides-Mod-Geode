#include <Geode/Geode.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/binding/LevelInfoLayer.hpp>
#include <Geode/binding/LevelManagerDelegate.hpp>
using namespace geode::prelude;

class BsideLayer : public cocos2d::CCLayer, public LevelManagerDelegate {
private:
GauntletType m_gauntletType;
int m_levelIds[5];
bool m_loadingNames = true;
int m_namesToLoad = 5;
	
public:
static cocos2d::CCScene* scene(GauntletType type) {
    auto s = cocos2d::CCScene::create();

    auto layer = new BsideLayer();

    layer->m_gauntletType = type;

    if(layer->init()){
        layer->autorelease();
        s->addChild(layer);
    }

    return s;
}

	bool init() override {
		if (!cocos2d::CCLayer::init()) {
			return false;
		}
		auto size = cocos2d::CCDirector::sharedDirector()->getWinSize();
auto sprite = cocos2d::CCSprite::create("backgroundBside.png"_spr);  

if (sprite) {
    sprite->setPosition(ccp(size.width / 2, size.height / 2));
    sprite->setScale(1.3f);
    this->addChild(sprite, -1);
}
auto backSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
auto backBtn = CCMenuItemSpriteExtra::create(
    backSprite,
    this,
    menu_selector(BsideLayer::onBack)
);

backBtn->setPosition(ccp(25, size.height - 25));

		auto menu = cocos2d::CCMenu::create(backBtn, nullptr);
		menu->setPosition(0, 0);
		this->addChild(menu);

const char* gauntletName = "unknown";
	
		switch (m_gauntletType) {
	  case GauntletType::Fire:
        gauntletName = "Fire";
        m_levelIds[0] = 41007125; m_levelIds[1] = 73963102; m_levelIds[2] = 83730530; m_levelIds[3] = 67069282; m_levelIds[4] = 115370944;
        break;

    case GauntletType::Ice:
        gauntletName = "Ice";
        m_levelIds[0] = 85928041; m_levelIds[1] = 96226630; m_levelIds[2] = 32973942; m_levelIds[3] = 89423740; m_levelIds[4] = 110479533;
        break;

    case GauntletType::Poison:
        gauntletName = "Poison";
        m_levelIds[0] = 75289880; m_levelIds[1] = 60787499; m_levelIds[2] = 117389317; m_levelIds[3] = 61974149; m_levelIds[4] = 110416299;
        break;

    case GauntletType::Shadow:
        gauntletName = "Shadow";
        m_levelIds[0] = 110928098; m_levelIds[1] = 80736286; m_levelIds[2] = 2891681; m_levelIds[3] = 128686302; m_levelIds[4] = 109540067;
        break;

    case GauntletType::Lava:
        gauntletName = "Lava";
        m_levelIds[0] = 134933219; m_levelIds[1] = 140359877; m_levelIds[2] = 91942382; m_levelIds[3] = 96872233; m_levelIds[4] = 109513812;;
        break;

        case GauntletType::Bonus:
        gauntletName = "Bonus";
        m_levelIds[0] = 105325610; m_levelIds[1] = 89588848; m_levelIds[2] = 93320781; m_levelIds[3] = 137266721; m_levelIds[4] = 114784521;;
        break;    

default:
return false;
		}
		std::string title = std::string(gauntletName) + " Gauntlet Side B";

auto titleLabel = cocos2d::CCLabelBMFont::create(
    title.c_str(),
    "bigFont.fnt"
);
titleLabel->setColor(ccc3(255, 215, 0));
titleLabel->setScale(0.8f);

titleLabel->setPosition(ccp(size.width / 2, size.height - 30));
this->addChild(titleLabel);


		// reminder to change these ugly ahh sprites one day because low quality qwq (make them 1.6x bigger to)
        

        const char* buttonSprites[5] = {
    "level1.png"_spr,
    "level2.png"_spr,
    "level3.png"_spr,
    "level4.png"_spr,
    "level5.png"_spr
};
		for (int i = 0; i < 5; i++) {
		auto sprite = cocos2d::CCSprite::create(buttonSprites[i]);
sprite->setScale(1.6f); 

auto levelBtn = CCMenuItemSpriteExtra::create(
    sprite,
    this,
    menu_selector(BsideLayer::onlevelbutton)
);
			levelBtn->setTag(m_levelIds[i]);
levelBtn->setPosition(ccp(75 + (i * 90), 83 + ((i % 2) * 110)));

			menu->addChild(levelBtn);

		}
		m_loadingNames = true;

auto glm = GameLevelManager::sharedState();
glm->m_levelManagerDelegate = this;

for (int i = 0; i < 5; i++) {

    auto search = GJSearchObject::create(
        SearchType::Search,
        std::to_string(m_levelIds[i])
    );

    glm->getOnlineLevels(search);
}

this->setKeypadEnabled(true);

		return true;
	}

	void onBack(cocos2d::CCObject*) {
    if (cocos2d::CCDirector::sharedDirector()->getRunningScene()) {
        cocos2d::CCDirector::sharedDirector()->popScene();
    }
	}

    void keyBackClicked() override {
    this->onBack(nullptr);
}


	void onlevelbutton(cocos2d::CCObject* sender) {
    auto btn = typeinfo_cast<cocos2d::CCNode*>(sender);
    if (!btn)
        return;

    int levelId = btn->getTag();

    auto search = GJSearchObject::create(
        SearchType::Search,
        std::to_string(levelId)
    );

    auto glm = GameLevelManager::sharedState();

    glm->m_levelManagerDelegate = this;
    glm->getOnlineLevels(search);
}

void loadLevelsFinished(cocos2d::CCArray* levels, char const* key) override {

  if (!levels || levels->count() == 0) {
    loadLevelsFailed(key);
    return;
}
    auto level = typeinfo_cast<GJGameLevel*>(levels->objectAtIndex(0));

    if (!level)
        return;

if (m_loadingNames) {

    auto label = cocos2d::CCLabelBMFont::create(
        level->m_levelName.c_str(),
        "bigFont.fnt"
    );

        label->setScale(0.35f); // look into robtop text longer = smaller shorter = bigger idk to lazy rn
   
        for (int i = 0; i < 5; i++) {

            if (level->m_levelID == m_levelIds[i]) {

                label->setPosition(
                    ccp(
                        75 + (i * 90),
                        83 + ((i % 2) * 110) + 45
                    )
                );

                this->addChild(label);
                break;
            }
        }

            m_namesToLoad--;

    if (m_namesToLoad <= 0) {
        m_loadingNames = false;
    }
        return;
    }

    auto infoLayer = LevelInfoLayer::create(level, false);

    auto scene = cocos2d::CCScene::create();
    scene->addChild(infoLayer);

cocos2d::CCDirector::sharedDirector()->pushScene(
    cocos2d::CCTransitionFade::create(0.25f, scene)
);
}

void loadLevelsFailed(char const* key) override {
    if (m_loadingNames) {
        m_namesToLoad--;
        if (m_namesToLoad <= 0) {
            m_loadingNames = false;
        }
    }
}

void onExit() override {
    auto glm = GameLevelManager::sharedState();

    if (glm->m_levelManagerDelegate == this)
        glm->m_levelManagerDelegate = nullptr;

    CCLayer::onExit();
}

}; 
#include <Geode/modify/GauntletLayer.hpp>

static CCMenu* findMenuRecursive(cocos2d::CCNode* node) {
	if (!node) {
		return nullptr;
	}

	if (auto menu = typeinfo_cast<CCMenu*>(node)) {
		return menu;
	}

	auto children = node->getChildren();
	if (!children) {
		return nullptr;
	}

	for (unsigned i = 0; i < children->count(); ++i) {
		auto child = typeinfo_cast<cocos2d::CCNode*>(children->objectAtIndex(i));
		if (!child) {
			continue;
		}

		if (auto found = findMenuRecursive(child)) {
			return found;
		}
	}

	return nullptr;
}

class $modify(GauntletBButton, GauntletLayer) {
	struct Fields {
    GauntletType gauntletType;
	};
	
	bool init(GauntletType type) {
		if (!GauntletLayer::init(type)) {
			return false;
		}

		m_fields->gauntletType = type;
		this->scheduleOnce(schedule_selector(GauntletBButton::addButton), 0.0f);
		return true;
	}

        static bool hasBside(GauntletType type) {
    switch (type) {
        case GauntletType::Fire:
        case GauntletType::Ice:
        case GauntletType::Poison:
        case GauntletType::Shadow:      
        case GauntletType::Lava: 
        case GauntletType::Bonus:    
            return true;

        default:
            return false;
    }
}

void addButton(float) {

    if (!hasBside(m_fields->gauntletType)) {
        log::debug("No B-Side available for this gauntlet");
        return;
    }

    auto myButton = CCMenuItemSpriteExtra::create(
        CCSprite::create("Bside_Button.png"_spr),
        this,
        menu_selector(GauntletBButton::onMyButton)
    );

if (!myButton) {
    log::error("B-sides Button Failed to Create");
    return;
}
		auto menuNode = this->getChildByID("bottom-menu");
		auto menu = typeinfo_cast<CCMenu*>(menuNode);
		if (!menu) {
			menu = findMenuRecursive(this);
		}

		if (menu) {
			menu->addChild(myButton);
			myButton->setPosition(ccp(490, 280));
			myButton->setID("bside-button"_spr);
			menu->updateLayout();
			log::debug("Added B-Sides button to GauntletLayer menu after init");
		} else {
			myButton->setPosition(ccp(490, 280));
			this->addChild(myButton);
			log::warn("No CCMenu found; added button directly to GauntletLayer");
		}
	}

void onMyButton(CCObject*) {
    auto scene = BsideLayer::scene(m_fields->gauntletType);

    cocos2d::CCDirector::sharedDirector()->pushScene(
        cocos2d::CCTransitionCrossFade::create(0.3f, scene)
    );
}
};

// its 4 am i need help