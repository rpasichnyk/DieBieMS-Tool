/*
    Original copyright 2018 Benjamin Vedder benjamin@vedder.se and the VESC Tool project ( https://github.com/vedderb/vesc_tool )
    Now forked to:
    Danny Bokma github@diebie.nl

    This file is part of BMS Tool.

    DieBieMS Tool is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DieBieMS Tool is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "startupwizard.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QScrollBar>
#include <QDesktopServices>

StartupWizard::StartupWizard(BMSInterface *vesc, QWidget *parent) : QWizard(parent)
{
    QSettings().setValue("intro_done", false);

    setPage(Page_Intro, new StartupIntroPage(vesc));
    setPage(Page_Usage, new StartupUsagePage(vesc));
    setPage(Page_Warranty, new StartupWarrantyPage(vesc));
    setPage(Page_Conclusion, new StartupConclusionPage(vesc));

    setStartId(Page_Intro);
    setWizardStyle(ModernStyle);
    setPixmap(QWizard::LogoPixmap, QPixmap("://res/icon.png").
              scaled(40, 40,
                     Qt::KeepAspectRatio,
                     Qt::SmoothTransformation));
    resize(800, 450);

    setWindowTitle(tr("VESC Tool Introduction"));

    mSideLabel = new AspectImgLabel(Qt::Vertical);
    mSideLabel->setPixmap(QPixmap("://res/logo_intro_wizard.png"));
    mSideLabel->setScaledContents(true);
    setSideWidget(mSideLabel);

    connect(this, SIGNAL(currentIdChanged(int)),
            this, SLOT(idChanged(int)));
}

void StartupWizard::idChanged(int id)
{
    if (id == Page_Intro || id == Page_Conclusion) {
        setSideWidget(mSideLabel);
        mSideLabel->setVisible(true);
    } else {
        setSideWidget(0);
    }
}

StartupIntroPage::StartupIntroPage(BMSInterface *vesc, QWidget *parent)
    : QWizardPage(parent)
{
    mBrowser = new VTextBrowser;
    mBrowser->setFrameStyle(QFrame::NoFrame);
    mBrowser->viewport()->setAutoFillBackground(false);

    ConfigParam *p = vesc->infoConfig()->getParam("wizard_startup_intro");
    if (p != 0) {
        setTitle(p->longName);
        mBrowser->setHtml(p->description);
    } else {
        setTitle("Text not found.");
        mBrowser->setText("Text not found.");
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mBrowser);
    setLayout(layout);
}

int StartupIntroPage::nextId() const
{
    return StartupWizard::Page_Usage;
}

StartupUsagePage::StartupUsagePage(BMSInterface *vesc, QWidget *parent)
    : QWizardPage(parent)
{
    mBrowser = new VTextBrowser;
    mAcceptBox = new QCheckBox("Yes, I understand and accept");

    ConfigParam *p = vesc->infoConfig()->getParam("wizard_startup_usage");
    if (p != 0) {
        setTitle(p->longName);
        setSubTitle(p->valString);
        mBrowser->setHtml(p->description);
    } else {
        setTitle("Text not found.");
        setSubTitle("Text not found");
        mBrowser->setText("Text not found.");
    }

    registerField("usageAccept*", mAcceptBox);

    connect(mBrowser->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(scrollValueChanged(int)));
    connect(mBrowser->verticalScrollBar(), SIGNAL(rangeChanged(int,int)),
            this, SLOT(scrollRangeChanged()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mBrowser);
    layout->addWidget(mAcceptBox);
    setLayout(layout);
}

int StartupUsagePage::nextId() const
{
    return StartupWizard::Page_Warranty;
}

void StartupUsagePage::scrollValueChanged(int value)
{
    if (value == mBrowser->verticalScrollBar()->maximum()) {
        mAcceptBox->setEnabled(true);
    } else {
        mAcceptBox->setEnabled(false);
        mAcceptBox->setChecked(false);
    }
}

void StartupUsagePage::scrollRangeChanged()
{
    scrollValueChanged(mBrowser->verticalScrollBar()->value());
}

StartupWarrantyPage::StartupWarrantyPage(BMSInterface *vesc, QWidget *parent)
    : QWizardPage(parent)
{
    mBrowser = new VTextBrowser;
    mAcceptBox = new QCheckBox("Yes, I understand and accept");

    ConfigParam *p = vesc->infoConfig()->getParam("wizard_startup_warranty");
    if (p != 0) {
        setTitle(p->longName);
        setSubTitle(p->valString);
        mBrowser->setHtml(p->description);
    } else {
        setTitle("Text not found.");
        setSubTitle("Text not found");
        mBrowser->setText("Text not found.");
    }

    registerField("warrantyAccept*", mAcceptBox);

    connect(mBrowser->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(scrollValueChanged(int)));
    connect(mBrowser->verticalScrollBar(), SIGNAL(rangeChanged(int,int)),
            this, SLOT(scrollRangeChanged()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mBrowser);
    layout->addWidget(mAcceptBox);
    setLayout(layout);
}

int StartupWarrantyPage::nextId() const
{
    return StartupWizard::Page_Conclusion;
}

void StartupWarrantyPage::scrollValueChanged(int value)
{
    if (value == mBrowser->verticalScrollBar()->maximum()) {
        mAcceptBox->setEnabled(true);
    } else {
        mAcceptBox->setEnabled(false);
        mAcceptBox->setChecked(false);
    }
}

void StartupWarrantyPage::scrollRangeChanged()
{
    scrollValueChanged(mBrowser->verticalScrollBar()->value());
}

StartupConclusionPage::StartupConclusionPage(BMSInterface *vesc, QWidget *parent)
    : QWizardPage(parent)
{
    mBrowser = new VTextBrowser;
    mBrowser->setFrameStyle(QFrame::NoFrame);
    mBrowser->viewport()->setAutoFillBackground(false);

    ConfigParam *p = vesc->infoConfig()->getParam("wizard_startup_conclusion");
    if (p != 0) {
        setTitle(p->longName);
        mBrowser->setHtml(p->description);
    } else {
        setTitle("Text not found.");
        mBrowser->setText("Text not found.");
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mBrowser);
    setLayout(layout);
}

int StartupConclusionPage::nextId() const
{
    return -1;
}

bool StartupConclusionPage::validatePage()
{
    QSettings().setValue("intro_done", true);
    return true;
}
