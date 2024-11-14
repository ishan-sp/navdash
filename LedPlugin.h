#ifndef LEDPLUGIN_H
#define LEDPLUGIN_H

#include <QObject>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#include "Led.h"

// Define export/import macro
#if defined(LEDPLUGIN_LIBRARY)
#define LEDPLUGIN_EXPORT Q_DECL_EXPORT
#else
#define LEDPLUGIN_EXPORT Q_DECL_IMPORT
#endif

class LEDPLUGIN_EXPORT LedPlugin : public QObject, public QDesignerCustomWidgetInterface {
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit LedPlugin(QObject *parent = nullptr);

    bool isContainer() const override;
    bool isInitialized() const override;
    QIcon icon() const override;
    QString domXml() const override;
    QString group() const override;
    QString includeFile() const override;
    QString name() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
    QWidget *createWidget(QWidget *parent) override;
    void initialize(QDesignerFormEditorInterface *core) override;

private:
    bool initialized;
};

#endif // LEDPLUGIN_H
