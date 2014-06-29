#include "../include/Config.h"
#include "../include/PlayerCommon.h"

using namespace plrCommon;
using namespace boost;

Config::Config()
{
    //ctor
}

Config::~Config()
{
    //dtor
}

void Config::loadTextFileConfigurations(string fName) {
    playerLog("Reading manual configurations..");
    vector<string> linesOfFile;
    std::ifstream confTxtFile(fName.c_str());
    std::ifstream ifs(fName.c_str());
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    typedef vector< iterator_range<string::iterator> > find_vector_type;
    typedef vector< string > split_vector_type;
    split_vector_type splitVec; // #2: Search for tokens .. now just line breaks.
    split( splitVec, content, is_any_of("\n"), token_compress_on );
    for (int i=0;i<splitVec.size();i++) {
        string line = splitVec.at(i);
        if (line.substr(0,1)!="#") {
            std::size_t sep = line.find("=");
            string param = line.substr(0,sep);
            string val   = line.substr(sep+1);
            // Add more parameters as needed..
            if (param=="PROFILE_CHANNEL_1")
                bwMeter.PROFILE_CHANNEL_1 = strToInt(val);
            if (param=="PROFILE_CHANNEL_2")
                bwMeter.PROFILE_CHANNEL_2 = strToInt(val);
            if (param=="AMOUNT_OF_PEAKS_IN_CHANNEL")
                AMOUNT_OF_PEAKS_IN_CHANNEL = strToInt(val);
            if (param=="SMOOTH_GRAPH_DIVISION_IN_SECS")
                SMOOTH_GRAPH_DIVISION_IN_SECS = strToInt(val);
            if (param=="DJModeTopRankingRate")
                DJModeTopRankingRate = strToDes(val);
            if (param=="DJModeLastPlayedItemsListSize")
                DJModeLastPlayedItemsListSize = strToDes(val);
            if (param=="autoSaveConfigurationTime")
                autoSaveConfigurationTime = strToInt(val);
        }
    }

    // Set some GUI Element's text to match the changes.
    djModeUseXAxis.setText("Use X-axis ("+getGraphDescriptionText(bwMeter.PROFILE_CHANNEL_1)+")");
    djModeUseXAxis.setToolTipTextOnly("If this is enabled DJ Mode will find songs according "+getGraphDescriptionText(bwMeter.PROFILE_CHANNEL_1)+"-channel of recordings.");
    djModeUseYAxis.setText("Use X-axis ("+getGraphDescriptionText(bwMeter.PROFILE_CHANNEL_2)+")");
    djModeUseYAxis.setToolTipTextOnly("If this is enabled DJ Mode will find songs according "+getGraphDescriptionText(bwMeter.PROFILE_CHANNEL_2)+"-channel of recordings.");

    actOp1.setText("Use X-axis ("+getGraphDescriptionText(bwMeter.PROFILE_CHANNEL_1)+")");
    actOp2.setText("Use Y-axis ("+getGraphDescriptionText(bwMeter.PROFILE_CHANNEL_2)+")");

}

void Config::updateRunTimeTaskes() {
    if (al_get_time() - lastTimeAutoConfigTime > autoSaveConfigurationTime && autoSaveConfigurationTime > 0) {
        lastTimeAutoConfigTime = al_get_time();
        config.save();
    }
}

void Config::load() {
    lastTimeAutoConfigTime=0;
    load(configurationFileName);
    loadTextFileConfigurations(manualConfigurationFileName);
}

void Config::load(string confFile) {
    playerLog("Loading configurations..");
    try {
        if ( boost::filesystem::exists( confFile.c_str() ) )
        {
            std::ifstream ifs(confFile.c_str());
            boost::archive::text_iarchive ia(ifs);
            ia >> config;
            loadVariablesAfterLoad();
        }
        else
        {
            playerLog("Configuration file not created yet. It's normal for first time.");
        }
    } catch (int e) {
        playerLog("Old version of config? No problem. Just save.");
    }
    loadVariablesAfterLoad();
}

void Config::loadVariablesAfterLoad() {

    /*for(size_t i = 0; i < configVars.size(); ++i)
    {
        playerLog("Log " + configVars[i].varVal);
    }*/
    vector<GUIElement*> elements = elementList.getElements();
    for(size_t i = 0; i < configVars.size(); ++i)
    {
        for(size_t e = 0; e < elements.size(); ++e)
        {
            if (elements[e]->isStorable() && configVars[i].varVal!="-1")
            {
                if (dynamic_cast<GUIRollButton*>(elements[e]) != NULL)
                {
                    GUIRollButton* b = dynamic_cast<GUIRollButton*>(elements[e]);
                    if (intToStr(b->getId()) == configVars[i].varName)
                    {
                        double temp = ::atof(configVars[i].varVal.c_str());
                        b->setValue(temp);
                    }
                }
                if (dynamic_cast<GUIRadioButton*>(elements[e]) != NULL)
                {
                    GUIRadioButton* b = dynamic_cast<GUIRadioButton*>(elements[e]);
                    if (intToStr(b->getId()) == configVars[i].varName)
                    {
                        int temp = ::atoi(configVars[i].varVal.c_str());
                        b->setCheckedIndex(temp);
                    }
                }
                if (dynamic_cast<GUICheckBox*>(elements[e]) != NULL)
                {
                    GUICheckBox* b = dynamic_cast<GUICheckBox*>(elements[e]);
                    if (intToStr(b->getId()) == configVars[i].varName)
                    {
                        bool temp = strToBool(configVars[i].varVal.c_str());
                        b->setChecked(temp);
                    }
                }
                if (dynamic_cast<GUIXYPicker*>(elements[e]) != NULL)
                {
                    GUIXYPicker* b = dynamic_cast<GUIXYPicker*>(elements[e]);
                    if (intToStr(b->getId()) == configVars[i].varName)
                    {
                        string line = configVars[i].varVal;
                        std::size_t sep = line.find("|");
                        double paramX = strToDes(line.substr(0,sep));
                        double paramY = strToDes(line.substr(sep+1));
                        b->setValues(paramX,paramY);
                    }
                }

            }
        }
    }
}

void Config::bringVariablesForSaving() {
    // Initialize all elements from element list that can be stored.
    vector<GUIElement*> elements = elementList.getElements();
    configVars.clear();
    for(size_t i = 0; i < elements.size(); ++i)
    {
         if (elements[i]->isStorable()) {
            if (dynamic_cast<GUIRollButton*>(elements[i]) != NULL)
            {
                GUIRollButton* b = dynamic_cast<GUIRollButton*>(elements[i]);
                configVars.push_back(ConfigVar(intToStr(b->getId()), (desToStr(b->getConvertedValue())+"").c_str() ));
            }
            if (dynamic_cast<GUIRadioButton*>(elements[i]) != NULL)
            {
                GUIRadioButton* b = dynamic_cast<GUIRadioButton*>(elements[i]);
                configVars.push_back(ConfigVar(intToStr(b->getId()), intToStr(b->getCheckedIndex()).c_str() ));
            }
            if (dynamic_cast<GUICheckBox*>(elements[i]) != NULL)
            {
                GUICheckBox* b = dynamic_cast<GUICheckBox*>(elements[i]);
                configVars.push_back(ConfigVar(intToStr(b->getId()), boolToStr(b->isChecked()).c_str() ));
            }
            if (dynamic_cast<GUIXYPicker*>(elements[i]) != NULL)
            {
                GUIXYPicker* b = dynamic_cast<GUIXYPicker*>(elements[i]);
                configVars.push_back(ConfigVar(intToStr(b->getId()), (desToStr(b->getValX()) + "|" + desToStr(b->getValY())).c_str() ));
            }
         }
    }
}

void Config::save() {
    save(configurationFileName);
}

void Config::save(string confFile) {
    bringVariablesForSaving();
    std::ofstream ofs(confFile.c_str());
    {
        boost::archive::text_oarchive oa(ofs);
        oa << config;
    }
}
