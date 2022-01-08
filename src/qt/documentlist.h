// Copyright (c) 2019 Softwarebuero Krekeler

// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DOCUMENTLIST_H
#define DOCUMENTLIST_H

#include "platformstyle.h"
#include "walletmodel.h"
#include "primitives/document.h"

#include <QDateTime>
#include <QStringListModel>
#include <QWidget>

namespace Ui {
    class DocumentList;
}

class ClientModel;
class WalletModel;

class Document : public QObject {
    Q_OBJECT
public:
    explicit Document(const QString docFileName);
    ~Document();

    QString name;
    QString filename;
    QString descfilename;
    int version;

    void loadDescription();
    QString documentRevision();
    QString getInformationHtml();
    QString writeToBlockchain(const std::string &comprguid, const std::string &indexhash,
                              const std::string &filehash, const std::string &attrhash);
private:
    QString guid;
    CDocumentHash indexhash;
    CDocumentHash filehash;
    CDocumentHash attrhash;
    QString txid;
    int filesize;

    QDateTime savetime;
    int minConfirms;
};

class DocumentList : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentList(const PlatformStyle *_platformStyle, QWidget *parent = nullptr);
    ~DocumentList();
    const PlatformStyle *platformStyle;

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *model);

    void LoadFiles();
    void handleNewFiles(const QStringList newFiles);

private:
    Ui::DocumentList *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
    QStringListModel *documentModel;

    QString fileDir() const;
    QString getFileName(const QModelIndex &index, const bool fullPath);
    QString selectedFileName(const bool fullPath);
    void selectDocument(const QString docFileName);

    bool TransactionConfirmDlg(const QString docName, const double txFee);
    QString addFile(const QString srcFile);
    QString addFiles(const QStringList srcFiles);
    void documentRevision(const QString docFileName);

private Q_SLOTS:
    void onlistViewDocumentsChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_listViewDocuments_doubleClicked(const QModelIndex &index);
    void on_textBrowserRevision_anchorClicked(const QUrl &arg1);
    void on_pushButtonAddFile_clicked();
    void on_pushButtonOpenFile_clicked();
    void on_pushButtonRevision_clicked();
};

#endif // DOCUMENTLIST_H
