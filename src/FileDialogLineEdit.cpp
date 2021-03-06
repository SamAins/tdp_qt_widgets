#include "tdp_qt_widgets/FileDialogLineEdit.h"

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace tdp_qt_widgets
{
//##################################################################################################
struct FileDialogLineEdit::Private
{
  FileDialogLineEdit* q;

  QLineEdit* lineEdit{nullptr};
  QPushButton* pushButton{nullptr};

  QString initialDirectory;
  QString filter;
  FileDialogLineEdit::Mode mode{FileDialogLineEdit::DirectoryMode};

  //################################################################################################
  Private(FileDialogLineEdit* q_):
    q(q_)
  {

  }

  //################################################################################################
  QString dir()
  {
    if(lineEdit->text().isEmpty())
      return initialDirectory;

    if(mode == FileDialogLineEdit::DirectoryMode)
      return lineEdit->text();

    QStringList parts = lineEdit->text().split("/");
    parts.removeLast();
    return parts.join('/');
  }
};

//##################################################################################################
FileDialogLineEdit::FileDialogLineEdit(QWidget* parent):
  QWidget(parent),
  d(new Private(this))
{
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  d->lineEdit = new QLineEdit();
  layout->addWidget(d->lineEdit);

  d->pushButton = new QPushButton("Select");
  d->pushButton->setFixedWidth(100);

  connect(d->pushButton, &QPushButton::clicked, [&]()
  {
    QString str;

    if(d->mode == FileDialogLineEdit::DirectoryMode)
      str = QFileDialog::getExistingDirectory(d->q->parentWidget(), "Select Directory", d->dir());
    else if(d->mode == FileDialogLineEdit::OpenFileMode)
      str = QFileDialog::getOpenFileName(d->q->parentWidget(), "Load File", d->dir(), d->filter);
    else
      str = QFileDialog::getSaveFileName(d->q->parentWidget(), "Save File", d->dir(), d->filter);

    if(!str.isEmpty())
    {
      d->lineEdit->setText(str);
      emit d->q->selectionChanged();
    }
  });

  layout->addWidget(d->pushButton);
}

//##################################################################################################
FileDialogLineEdit::~FileDialogLineEdit()
{
  delete d;
}

//##################################################################################################
void FileDialogLineEdit::setText(const QString& text)
{
  d->lineEdit->setText(text);
}

//##################################################################################################
QString FileDialogLineEdit::text()const
{
  return d->lineEdit->text();
}

//##################################################################################################
void FileDialogLineEdit::setInitialDirectory(const QString& dir)
{
  d->initialDirectory = dir;
}

//##################################################################################################
void FileDialogLineEdit::setMode(FileDialogLineEdit::Mode mode)
{
  d->mode = mode;
}

//##################################################################################################
void FileDialogLineEdit::setFilter(const QString& filter)
{
  d->filter = filter;
}


}
