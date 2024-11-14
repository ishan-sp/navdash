#include "LedPlugin.h"
#include <QtPlugin>

Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")

LedPlugin::LedPlugin(QObject *parent)
    : QObject(parent), initialized(false) {}

bool LedPlugin::isContainer() const {
    return false;
}

bool LedPlugin::isInitialized() const {
    return initialized;
}

QIcon LedPlugin::icon() const {
    return QIcon(); // Optionally provide an icon for your widget
}

QString LedPlugin::domXml() const {
    return "<widget class=\"Led\" name=\"led\">\n"
           "</widget>\n";
}

QString LedPlugin::group() const {
    return "Custom Widgets";
}

QString LedPlugin::includeFile() const {
    return "Led.h";
}

QString LedPlugin::name() const {
    return "Led";
}

QString LedPlugin::toolTip() const {
    return "Custom LED Widget";
}

QString LedPlugin::whatsThis() const {
    return "A custom LED widget";
}

QWidget *LedPlugin::createWidget(QWidget *parent) {
    return new Led(parent);
}

void LedPlugin::initialize(QDesignerFormEditorInterface * /* core */) {
    if (initialized)
        return;
    initialized = true;
}
