#include <Geode/Geode.hpp>
#include <Geode/modify/GameSettingsLayer.hpp>
#include <Geode/modify/SecretLayer2.hpp>

using namespace geode::prelude;

int bumpOptions = 20;
int bumpValues[20] = { 0, 1, 9, 12, 22, 39, 50, 67, 76, 98, 100, 128, 149, 255, 352, 512, 727, 940, 1000, 1003 };
std::string bumpMessages[20] = {
	/* 0 */ "Where did all the bumpscosity go?",
	/* 1 */ "Really, just a single bumpscosit?",
	/* 9 */ "Do you feel a lack of bumpscosity in here?",
	/* 12 */ "Ah, just a perfect breeze of bumpscosity",
	/* 22 */ "I sense bumpscosity in you...",
	/* 39 */ "It's comfortably bumpscocious in here",
	/* 50 */ "What a pleasant amount of bumpscosity we've got today!",
	/* 67 */ "Come on in, the bumpscosity's great!",
	/* 76 */ "Do you feel more bumpscocious than usual?",
	/* 98 */ "The weather forecast's calling for another bumpscocious day.",
	/* 100*/ "One hundred whole bumpscosits? That's quite a lot!",
	/* 128*/ "Who turned up the bumpscosity so high?",
	/* 149*/ "I don't remember there being this much bumpscosity...",
	/* 255*/ "Did someone touch the bumpscosity again?",
	/* 352*/ "Not even this much bumpscosity will stop me from doing my job!",
	/* 512*/ "Watch out, it's extremely bumpscocious today!",
	/* 727*/ "How can you stand this much bumpscosity?",
	/* 940*/ "My god, the bumpscosity in here is absolutely overwhelming!",
	/*1000*/ "GAH! I can't handle this much bumpscosity!",
	/*1003*/ "Anyone else hear feel that ominous amount of bumpscosity? No? Just me?"
};

int getBumpscosityIndex(float percentage) {
	if (percentage <= 0) return 0;
	else if (percentage >= 1) return (bumpOptions - 1);
	else return roundf(1 + ((bumpOptions - 3) * percentage));
}

class $modify(CustomGameSettingsLayer, GameSettingsLayer) {
	struct Fields {
		Slider* bumpSlider;
		CCLabelBMFont* bumpText;
	};

	static GameSettingsLayer* create(GJBaseGameLayer* p0, int p1) {

		GameSettingsLayer* gsl = GameSettingsLayer::create(p0, p1);

		auto winSize = CCDirector::sharedDirector()->getWinSize();
		float currentVal = Mod::get()->getSavedValue<float>("bumpscosity", 0);

		auto label = CCLabelBMFont::create("", "bigFont.fnt");
		auto center = (winSize.width / 2);
		auto vCenter = (winSize.height / 2);
		
		int settingsTab = 9;  // "other" tab, page 2

		float yOffset = 100.4f;
		int ySpacing = 22;

		float labelScale = 0.35f;
		float sliderScale = 0.56f;

		int xOffset = 95;
		int leftXOffset = -xOffset - 15;
		
		// on mobile, there's no scroll slider so we can just put bumpscosity there!
		#if defined(GEODE_IS_MOBILE)
			xOffset = leftXOffset;
		#endif

		// label
		label->setPosition({ center + xOffset, vCenter - yOffset });
		label->setScale(labelScale);
		label->setVisible(false);
		label->setID("bumpscosity-label"_spr);
		gsl->m_mainLayer->addChild(label);
		gsl->getPageContainer(settingsTab)->addObject(label);
		gsl->addObjectToGroup(label, 0);

		// slider
		auto slider = Slider::create(gsl, menu_selector(CustomGameSettingsLayer::onBumpscosityChange), sliderScale);
		slider->setPosition({ center + xOffset, vCenter - yOffset - ySpacing });
		slider->setValue(clampf(currentVal, 0, 1));
		slider->setVisible(false);
		slider->setID("bumpscosity-slider"_spr);
		gsl->m_mainLayer->addChild(slider);
		gsl->getPageContainer(settingsTab)->addObject(slider);
		gsl->addObjectToGroup(slider, 0);

		// move the clear plat saves button >:(
		#if !defined(GEODE_IS_MOBILE)
			CCNode* platButtonBase = gsl->m_buttonMenu->getChildByTag(-1004);
			if (platButtonBase) {
				bool shouldCenter = GameManager::get()->m_hasRP != 2;  // elder mods have an extra setting, shift button to account for it
				float btnPos = shouldCenter ? 0 : xOffset;
				CCMenuItemSpriteExtra* platButton = typeinfo_cast<CCMenuItemSpriteExtra*>(platButtonBase);
				platButton->m_baseScale = 0.8f;
				platButton->setScale(0.8f);
				platButton->setPosition({ btnPos, 48 });
			}
		#endif

		// store data
		auto gsl2 = modify_cast<CustomGameSettingsLayer*>(gsl);
		gsl2->m_fields->bumpSlider = slider;
		gsl2->m_fields->bumpText = label;
		gsl2->setBumpscosityLabel(currentVal);

		gsl->showTab(gsl->m_currentTab);

		return gsl;
	}

	void onBumpscosityChange(CCObject* sender) {
		float bumpVal = m_fields->bumpSlider->getThumb()->getValue();

		Mod::get()->setSavedValue<float>("bumpscosity", clampf(bumpVal, 0, 1));
		setBumpscosityLabel(bumpVal);

		return;
	}

	void setBumpscosityLabel(float percentage) {
		int bumpIndex = getBumpscosityIndex(percentage);

		m_fields->bumpText->setString(("Bumpscosity: " + std::to_string(bumpValues[bumpIndex])).c_str());
	}
};


class $modify(CustomSecretLayer2, SecretLayer2) {

	bool init() {
		SecretLayer2::init();
		if (rand() % 4 == 0) updateMessageLabel(getBumpscosityMessage());
		return true;
	}

	std::string getBasicMessage() {
		if (rand() % 12 == 0) return getBumpscosityMessage();
		else return SecretLayer2::getBasicMessage();
	}

	std::string getBumpscosityMessage() {
		float currentVal = Mod::get()->getSavedValue<float>("bumpscosity", 0);
		int bumpIndex = getBumpscosityIndex(currentVal);
		return bumpMessages[bumpIndex];
	}

};
