// Copyright (c) 2021 The Documentchain developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "information.h"
#include "ui_information.h"

#include "clientmodel.h"
#include "walletmodel.h"
#include "guiutil.h"

#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

/** local functions
*/
namespace {

const QString downloadRootURL = "https://documentchain.org/files/dms-core-integrations/information/";
    //QString downloadRootURL = "https://raw.githubusercontent.com/Krekeler/dms-core-integrations/main/information/";
      QString indexFileName = "categories.txt";
      std::string userNotesFileName = "mynotes.txt";

} // namespace


/** Information
*/

Information::Information(const PlatformStyle *_platformStyle, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Information),
    clientModel(0),
    walletModel(0),
    platformStyle(_platformStyle)
{
    ui->setupUi(this);
    categoryModel = new QStringListModel(this);
    ui->listViewCategory->setModel(categoryModel);

    filenameUserNotes = GUIUtil::boostPathToQString(GetDataDir() / userNotesFileName);
    categoryChanging = false;
    userNotesModified = false;
    defaultCharFormat = ui->textBrowserInfo->currentCharFormat();
    downloadURL = downloadRootURL + tr("en/");
    downloadFile(indexFileName); // one download at startup
}

void Information::setClientModel(ClientModel *model)
{
    this->clientModel = model;
}

void Information::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
}

Information::~Information()
{
    if (userNotesModified)
        saveUserNotes();

    delete ui;
}

void Information::downloadFile(QString fileName)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &Information::finishedDownloadFile);
    manager->get(QNetworkRequest(QUrl(downloadURL + fileName)));
}

void Information::finishedDownloadFile(QNetworkReply *reply)
{
    QString url = reply->url().toString();

    // do not overwrite mynotes if the user changes faster than the download takes
    if (isUserNotesSelected(ui->listViewCategory->currentIndex()))
        return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bts = reply->readAll();
        QString str(bts);
        if (url.endsWith("/" + indexFileName)) {
            setCategories(str);
        }
        else
            ui->textBrowserInfo->setHtml(str);
    }
    else {
        showErrorPage();
        if (url.endsWith("/" + indexFileName))
            setCategories("");
    }

    reply->deleteLater();
}

void Information::setCategories(const QString &cats)
{
    QStringList catlist = cats.split(QRegExp("(\\r\\n)|(\\n\\r)|\\r|\\n"), QString::SkipEmptyParts);
    catlist << tr("My Notes");
    categoryModel->setStringList(catlist);

    ui->listViewCategory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->listViewCategory->selectionModel(),
        SIGNAL(currentChanged(const QModelIndex& , const QModelIndex&)), this,
        SLOT(onlistViewCategoryChanged(const QModelIndex&, const QModelIndex&)),
        Qt::QueuedConnection);
}

void Information::showErrorPage()
{
    QString errorText = "<p>" + 
        tr("The recent information pages have not yet been loaded from %1").arg(downloadURL) +
        "</p><p>" +
        tr("You can find information on the project website at %1").arg("<a href=\"https://documentchain.org/\">documentchain.org</a>") +
        "</p>";
    ui->textBrowserInfo->setHtml(errorText);
}

void Information::saveUserNotes()
{
    QFile file(filenameUserNotes);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream output(&file);
        output << ui->textBrowserInfo->toPlainText();
        userNotesModified = false;
    }
    else
        QMessageBox::critical(NULL, tr("My Notes"), tr("Notes could not be saved to file."));
}

bool Information::isUserNotesSelected(const QModelIndex &selected)
{
    if (!selected.isValid())
        return false;
    return (categoryModel->stringList().at(selected.row()) == tr("My Notes"));
}

void Information::onlistViewCategoryChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (!current.isValid())
        return;

    categoryChanging = true;
    if (isUserNotesSelected(current)) {
        ui->textBrowserInfo->clear();
        ui->textBrowserInfo->setAcceptRichText(false);
        ui->textBrowserInfo->setCurrentCharFormat(defaultCharFormat);
        QFile file(filenameUserNotes);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream input(&file);
            ui->textBrowserInfo->setPlainText(input.readAll());
        }
        else
            ui->textBrowserInfo->setPlainText(tr("Here you can enter your own notes..."));
        userNotesModified = false;
        ui->textBrowserInfo->setReadOnly(false);
    }
    else {
        if (isUserNotesSelected(previous) && userNotesModified)
            saveUserNotes(); // save not only on exit

        ui->textBrowserInfo->setReadOnly(true);
        QString selcat = categoryModel->stringList().at(current.row());
        selcat = selcat.replace(" ", "-").toLower();
        downloadFile(selcat + ".html"); // setMarkdown requires Qt 5.14
    }
    categoryChanging = false;
}

void Information::on_textBrowserInfo_textChanged()
{
    if (isUserNotesSelected(ui->listViewCategory->currentIndex()) && !categoryChanging)
        userNotesModified = true;
}

void Information::on_textBrowserInfo_anchorClicked(const QUrl &arg1)
{
  /*QString cmd = arg1.toString();

    if (cmd == "internal-link")
        QMessageBox::information(NULL, tr("Link"), cmd);
    else*/
        QDesktopServices::openUrl(arg1);
}
