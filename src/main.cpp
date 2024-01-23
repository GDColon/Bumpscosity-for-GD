#include <Geode/Geode.hpp>
#include <Geode/modify/MoreOptionsLayer.hpp>

using namespace geode::prelude;

class $modify(CustomMoreOptionsLayer, MoreOptionsLayer) {
	Slider* bumpSlider;
	CCLabelBMFont* bumpText;
	int bumpOptions = 11;
	int bumpValues[11] = { 1, 9, 12, 22, 50, 76, 100, 128, 149, 727, 940 };

	bool init() {
		MoreOptionsLayer::init();

		CCLayer* mainLayer = MoreOptionsLayer::m_mainLayer;
        CCLayer* performance = as<CCLayer*>(mainLayer->getChildren()->objectAtIndex(6));

		auto winSize = CCDirector::sharedDirector()->getWinSize();
		float currentVal = Mod::get()->getSavedValue<float>("bumpscosity", 0);

		auto label = CCLabelBMFont::create("", "bigFont.fnt");
		auto center = (winSize.width / 2);

		label->setPosition({center, 64});
		label->setScale(0.5f);
		performance->addChild(label);

		auto slider = Slider::create(this, menu_selector(CustomMoreOptionsLayer::onBumpscosityChange), 0.8f);
		slider->setPosition({ center, 42 });
		slider->setValue(clampf(currentVal, 0, 1));
		performance->addChild(slider);

		m_fields->bumpSlider = slider;
		m_fields->bumpText = label;
		CustomMoreOptionsLayer::setBumpscosityLabel(currentVal);

		return true;
	} 

	void onBumpscosityChange(CCObject* sender) {
		float bumpVal = m_fields->bumpSlider->getThumb()->getValue();

		Mod::get()->setSavedValue<float>("bumpscosity", clampf(bumpVal, 0, 1));
		CustomMoreOptionsLayer::setBumpscosityLabel(bumpVal);

		return;
	}

	int getBumpscosityValue(float percentage) {
		if (percentage <= 0) return 0;
		else if (percentage >= 1) return 1000;

		int valIndex = roundf((m_fields->bumpOptions - 1) * percentage);
		return m_fields->bumpValues[valIndex];
	}

	void setBumpscosityLabel(float percentage) {
		int bumpNum = getBumpscosityValue(percentage);

		m_fields->bumpText->setString(("Bumpscosity: " + std::to_string(bumpNum)).c_str());
	}
};
