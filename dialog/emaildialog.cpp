#include "emaildialog.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QRegExp>
#include "global.h"

extern Global global;

EmailDialog::EmailDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowIcon(global.getIconResource(":emailIcon"));
    sendPressed = false;
    cancelPressed = true;
    setWindowTitle(tr("Send Email"));

    sendButton = new QPushButton(tr("Send"));
    cancelButton = new QPushButton(tr("Cancel"));

    QGridLayout *grid = new QGridLayout(this);
    setLayout(grid);
    QGridLayout *addressGrid = new QGridLayout();
    QGridLayout *noteGrid = new QGridLayout();
    QGridLayout *buttonGrid = new QGridLayout();
    grid->addLayout(addressGrid, 0,0);
    grid->addLayout(noteGrid, 1,0);
    grid->addLayout(buttonGrid,2,0);

    toLabel = new QLabel(this);
    toLabel->setText(tr("To:"));
    bccLabel = new QLabel(this);
    bccLabel->setText(tr("BCC:"));
    ccLabel = new QLabel(this);
    ccLabel->setText(tr("CC:"));
    subjectLabel = new QLabel(this);
    subjectLabel->setText(tr("Subject:"));
    noteLabel = new QLabel(this);
    noteLabel->setText(tr("Note:"));

    ccSelf = new QCheckBox(this);

    toAddress = new QLineEdit(this);
    ccAddress = new QLineEdit(this);
    bccAddress = new QLineEdit(this);
    subject = new QLineEdit(this);
    note = new QPlainTextEdit(this);

    int row = 0;
    addressGrid->addWidget(toLabel, row, 0);
    addressGrid->addWidget(toAddress, row++, 1);
    addressGrid->addWidget(ccLabel, row, 0);
    addressGrid->addWidget(ccAddress, row++, 1);
    addressGrid->addWidget(bccLabel, row, 0);
    addressGrid->addWidget(bccAddress, row++, 1);

    ccSelf->setText(tr("CC me on this email"));
    noteGrid->addWidget(ccSelf, 0,0);

    addressGrid->addWidget(subjectLabel, row, 0);
    addressGrid->addWidget(subject, row++,1);

    noteGrid->addWidget(noteLabel, 1, 0);
    noteGrid->addWidget(note, 2,0);

    buttonGrid->addWidget(cancelButton, 0,0);
    buttonGrid->addWidget(sendButton, 0,1);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendButtonPressed()));

    connect(toAddress, SIGNAL(textChanged(QString)), this, SLOT(toAddressChanged()));
    connect(ccAddress, SIGNAL(textChanged(QString)), this, SLOT(toAddressChanged()));
    connect(bccAddress, SIGNAL(textChanged(QString)), this, SLOT(toAddressChanged()));
    sendButton->setEnabled(false);
}




void EmailDialog::sendButtonPressed() {
    sendPressed = true;
    cancelPressed = false;
    close();
}



void EmailDialog::cancelButtonPressed() {
    cancelPressed = true;
    sendPressed = false;
    close();
}


void EmailDialog::toAddressChanged() {
    QString to, bcc, cc;
    to = toAddress->text().trimmed();
    bcc = bccAddress->text().trimmed();
    cc = ccAddress->text().trimmed();

    int toi = to.indexOf("@");
    int cci = cc.indexOf("@");
    int bcci = bcc.indexOf("@");

    if (toi <=0 && cci <= 0 && bcci <= 0)
        sendButton->setEnabled(false);
    else
        sendButton->setEnabled(true);
}



QStringList EmailDialog::tokenizeString(QString value) {
    QStringList values =  value.split(QRegExp(",|;"), QString::SkipEmptyParts);
    for (int i=0; i<values.size(); i++) {
        values[i] = values[i].trimmed();
    }
    return values;
}


QStringList EmailDialog::getToAddresses() {
    return tokenizeString(toAddress->text().trimmed());
}


QStringList EmailDialog::getCcAddresses() {
    return tokenizeString(ccAddress->text().trimmed());
}


QStringList EmailDialog::getBccAddresses() {
    return tokenizeString(bccAddress->text().trimmed());
}