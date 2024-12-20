#include <Geode/Geode.hpp>
#include <Geode/modify/MoreOptionsLayer.hpp>
#include <Geode/modify/SecretLayer2.hpp>

using namespace geode::prelude;

int bumpOptions = 15;
int bumpValues[15] = { 0, 1, 9, 12, 22, 39, 50, 76, 100, 128, 149, 255, 727, 940, 1000 };
std::string bumpMessages[15] = {
	/* 0 */ "Where did all the bumpscosity go?",
	/* 1 */ "Really, just a single bumpscosit?",
	/* 9 */ "Do you feel a lack of bumpscosity in here?",
	/* 12 */ "Ah, just a perfect breeze of bumpscosity",
	/* 22 */ "I sense bumpscosity in you...",
	/* 39 */ "It's comfortably bumpscocious in here",
	/* 50 */ "What a pleasant amount of bumpscosity we've got today!",
	/* 76 */ "Do you feel more bumpscocious than usual?",
	/* 100*/ "One hundred whole bumpscosits? That's quite a lot!",
	/* 128*/ "Who turned up the bumpscosity so high?",
	/* 149*/ "I don't remember there being this much bumpscosity...",
	/* 255*/ "Did someone touch the bumpscosity again?",
	/* 727*/ "How can you stand this much bumpscosity?",
	/* 940*/ "My god, the bumpscosity in here is absolutely overwhelming!",
	/*1000*/ "GAH! I can't handle this much bumpscosity!",
};

int getBumpscosityIndex(float percentage) {
	if (percentage <= 0) return 0;
	else if (percentage >= 1) return (bumpOptions - 1);
	else return roundf(1 + ((bumpOptions - 3) * percentage));
}

class $modify(CustomMoreOptionsLayer, MoreOptionsLayer) {
	struct Fields {
		Slider* bumpSlider;
		CCLabelBMFont* bumpText;
	};

	bool init() {
		MoreOptionsLayer::init();

		int settingsPage = 3;

		#if defined(GEODE_IS_ANDROID)
			settingsPage = 3;
		#endif

		CCLayer* mainLayer = MoreOptionsLayer::m_mainLayer;
        CCLayer* settingsLayer = as<CCLayer*>(mainLayer->getChildren()->objectAtIndex(settingsPage));

		auto winSize = CCDirector::sharedDirector()->getWinSize();
		float currentVal = Mod::get()->getSavedValue<float>("bumpscosity", 0);

		auto label = CCLabelBMFont::create("", "bigFont.fnt");
		auto center = (winSize.width / 2);
		auto vCenter = (winSize.height / 2);
		
		int yOffset = 95;
		int ySpacing = 22;

		label->setPosition({ center, vCenter - yOffset });
		label->setScale(0.5f);
		settingsLayer->addChild(label);

		auto slider = Slider::create(this, menu_selector(CustomMoreOptionsLayer::onBumpscosityChange), 0.8f);
		slider->setPosition({ center, vCenter - yOffset - ySpacing });
		slider->setValue(clampf(currentVal, 0, 1));
		settingsLayer->addChild(slider);

		m_fields->bumpSlider = slider;
		m_fields->bumpText = label;
		setBumpscosityLabel(currentVal);

		return true;
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

	gd::string getBasicMessage() {
		if (rand() % 15 == 0) return getBumpscosityMessage();
		else return SecretLayer2::getBasicMessage();
	}

	gd::string getBumpscosityMessage() {
		float currentVal = Mod::get()->getSavedValue<float>("bumpscosity", 0);
		int bumpIndex = getBumpscosityIndex(currentVal);
		return bumpMessages[bumpIndex];
	}

};