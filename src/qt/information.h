// Copyright (c) 2021 The Documentchain developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef INFORMATION_H
#define INFORMATION_H

#include "platformstyle.h"
#include "walletmodel.h"

#include <QStringListModel>
#include <QWidget>
#include <QNetworkReply>
#include <QTextCharFormat>

namespace Ui {
    class Information;
}

class ClientModel;
class WalletModel;

class Information : public QWidget
{
    Q_OBJECT

public:
    explicit Information(const PlatformStyle *_platformStyle, QWidget *parent = nullptr);
    ~Information();
    const PlatformStyle *platformStyle;

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *model);

private:
    Ui::Information *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
    QStringListModel *categoryModel;

    QString downloadURL;
    QString filenameUserNotes;
    QTextCharFormat defaultCharFormat;
    bool categoryChanging;
    bool userNotesModified;
    void downloadFile(QString fileName);
    void setCategories(const QString &cats);
    void showErrorPage();
    void saveUserNotes();
    bool isUserNotesSelected(const QModelIndex &current);

private Q_SLOTS:
    void finishedDownloadFile(QNetworkReply* reply);
    void onlistViewCategoryChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_textBrowserInfo_textChanged();
    void on_textBrowserInfo_anchorClicked(const QUrl &arg1);
};

#endif // INFORMATION_H
