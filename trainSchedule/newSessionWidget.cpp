#include "newSessionWidget.h"
#include "session.h"
#include "sessionType.h"
#include "duration.h"
#include "distance.h"
#include "sessionsDialog.h"
#include "mapAndControlWidget.h"

#include <QSharedPointer>
#include <QtWebChannel>
#include <QtDebug>

NewSessionWidget::NewSessionWidget(DaySchedule* ds, MapAndControlWidget* const macw, SessionsWidget *parent)
    : QWidget(parent),
      daySdl(ds),
      mapWidget(macw->getMapWidget()),
      mapControlWidget(macw->getMapControlWidget()),
      sessionsWidget(parent)
{
    createWidgets();
    createLayout();
    createConnections();
}

void NewSessionWidget::createWidgets() {
    typeLabel = new QLabel(tr("Type"));
    typeCombo = new QComboBox;
    typeCombo->addItem("run");
    typeCombo->addItem("bike");

    nameLabel = new QLabel(tr("Name"));
    nameLineEdit = new QLineEdit;
    // default name
    int numSessions = daySdl->getNumSessions();
    nameLineEdit->setText(QString("session %1").arg(numSessions+1));

    durationLabel = new QLabel(tr("Time"));
    durationSpinBox = new DurationSpinBox();

    distanceLabel = new QLabel(tr("Distance"));
    distanceSpinBox = new QDoubleSpinBox;
    distanceSpinBox->setValue(0);
    distanceSpinBox->setMaximum(400);
    //mapWidget->page()->addToJavaScriptWindowObject("QTDistanceSpinBoxNew", distanceSpinBox);

    QWebChannel * channel;
    if (mapWidget->page()->webChannel() != nullptr) {
        channel = mapWidget->page()->webChannel();
    } else {
        channel = new QWebChannel(mapWidget->page());
    }
    channel->registerObject(QString("QTDistanceSpinBoxNew"), distanceSpinBox);
    mapWidget->page()->setWebChannel(channel);

    descriptionLabel = new QLabel("Comment");
    descriptionEdit = new QTextEdit();

    applyButton = new QPushButton(tr("Apply"));

    mapButton = new QPushButton(tr("Show Map"));
}

void NewSessionWidget::createLayout() {
   topLayout = new QGridLayout;
   topLayout->addWidget(typeLabel, 0, 0);
   topLayout->addWidget(typeCombo, 0, 1);
   topLayout->addWidget(nameLabel, 1, 0);
   topLayout->addWidget(nameLineEdit, 1, 1);
   topLayout->addWidget(durationLabel, 2,0);
   topLayout->addWidget(durationSpinBox, 2, 1);
   topLayout->addWidget(distanceLabel, 3, 0);
   topLayout->addWidget(distanceSpinBox, 3, 1);
   topLayout->addWidget(descriptionLabel, 4, 0);
   topLayout->addWidget(descriptionEdit, 5, 0, 1, 2);
   topLayout->addWidget(applyButton, 6, 0, 1, 2);
   //topLayout->addWidget(mapButton, 4, 1);

   mainLayout = new QVBoxLayout;
   mainLayout->addLayout(topLayout);
   mainLayout->addStretch();

   setLayout(mainLayout);

   this->setFixedWidth(sizeHint().width());
}

void NewSessionWidget::createConnections() {
    connect(applyButton, SIGNAL(clicked()), this, SLOT(saveSession()));
}

void NewSessionWidget::updateDistance(double d) {
    d /= 1000;
    distanceSpinBox->setValue(d);
}

void NewSessionWidget::saveSession() {
    // validate input
    if (validateSession()) {
        qDebug() << "start save session";
        // read out values for new session
        Duration dur(durationSpinBox->value());
        Distance dist(distanceSpinBox->value());

        QSharedPointer<Session> newSession;
        int sessionType;
        switch(typeCombo->currentIndex()) {
            case 0: sessionType = SessionType::RUN;
                break;
            case 1: sessionType = SessionType::BIKE;
                break;
            default:sessionType = SessionType::RUN;
        }

        newSession = QSharedPointer<Session>(new Session(sessionType, dur, dist));
        newSession->setName(nameLineEdit->text());
        newSession->setDescription(descriptionEdit->toPlainText());

        mapWidget->fillMapRoute(newSession->getMapRoute());
        newSession->getMapRoute().setName(nameLineEdit->text());

        daySdl->addSession(newSession);

        completeReset();
        setWindowModified(true);

        int numSession = daySdl->getNumSessions();
        qDebug() << "start saveSession";
        sessionsWidget->showExistingSession(numSession-1);
        qDebug() << "end saveSession";
    }
}

bool NewSessionWidget::validateSession() const {
    //TODO: implement this
    return true;
}

void NewSessionWidget::reset() {
    typeCombo->setCurrentIndex(0);
    nameLineEdit->setText(QString("session %1").arg(daySdl->getNumSessions()+1));
    durationSpinBox->setValue(3600);
    distanceSpinBox->setValue(0);
    descriptionEdit->clear();
}

void NewSessionWidget::completeReset() {
    reset();
    mapControlWidget->reset();
}

void NewSessionWidget::enableControlWidget() {
    mapControlWidget->setEnabled(true);
}

void NewSessionWidget::disableControlWidget() {
    mapControlWidget->setEnabled(false);
}
