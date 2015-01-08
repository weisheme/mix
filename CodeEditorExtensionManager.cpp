/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file CodeEditorExtensionManager.cpp
 * @author Yann yann@ethdev.com
 * @date 2014
 * Ethereum IDE client.
 */

#include <QQuickItem>
#include <QGraphicsObject>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickTextDocument>
#include <libevm/VM.h>
#include "ConstantCompilationControl.h"
#include "AssemblyDebuggerControl.h"
#include "StateListView.h"
#include "AppContext.h"
#include "MixApplication.h"
#include "CodeModel.h"
#include "CodeEditorExtensionManager.h"

using namespace dev::mix;

CodeEditorExtensionManager::CodeEditorExtensionManager():
	m_appContext(static_cast<MixApplication*>(QApplication::instance())->context())
{
}

CodeEditorExtensionManager::~CodeEditorExtensionManager()
{
	m_features.clear();
}

void CodeEditorExtensionManager::loadEditor(QQuickItem* _editor)
{
	if (!_editor)
		return;

	QVariant doc = _editor->property("textDocument");
	if (doc.canConvert<QQuickTextDocument*>())
	{
		QQuickTextDocument* qqdoc = doc.value<QQuickTextDocument*>();
		if (qqdoc)
		{
			m_doc = qqdoc->textDocument();
		}
	}
}

void CodeEditorExtensionManager::initExtensions()
{
	initExtension(std::make_shared<ConstantCompilationControl>(m_appContext));
	std::shared_ptr<AssemblyDebuggerControl> debug = std::make_shared<AssemblyDebuggerControl>(m_appContext);
	std::shared_ptr<StateListView> stateList = std::make_shared<StateListView>(m_appContext);
	QObject::connect(m_doc, &QTextDocument::contentsChanged, [=]() { m_appContext->codeModel()->registerCodeChange(m_doc->toPlainText()); });

	initExtension(debug);
	initExtension(stateList);
}

void CodeEditorExtensionManager::initExtension(std::shared_ptr<Extension> _ext)
{
	if (!_ext->contentUrl().isEmpty())
	{
		try
		{
			if (_ext->getDisplayBehavior() == ExtensionDisplayBehavior::RightView)
				_ext->addContentOn(m_rightView);
			if (_ext->getDisplayBehavior() == ExtensionDisplayBehavior::HeaderView)
				_ext->addContentOn(m_headerView);
		}
		catch (...)
		{
			qDebug() << "Exception when adding tab into view.";
			return;
		}
	}
	_ext->start();
	m_features.append(_ext);
}

void CodeEditorExtensionManager::setEditor(QQuickItem* _editor)
{
	this->loadEditor(_editor);
	this->initExtensions();

	auto args = QApplication::arguments();
	if (args.length() > 1)
	{
		QString path = args[1];
		QFile file(path);
		if (file.exists() && file.open(QFile::ReadOnly))
			m_doc->setPlainText(file.readAll());
	}
}

void CodeEditorExtensionManager::setRightView(QQuickItem* _rightView)
{
	m_rightView = _rightView;
}

void CodeEditorExtensionManager::setHeaderView(QQuickItem* _headerView)
{
	m_headerView = _headerView;
}
