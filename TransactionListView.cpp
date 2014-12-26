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
/** @file TransactionListView.cpp
 * @author Arkadiy Paronyan arkadiy@ethdev.com
 * @date 2014
 * Ethereum IDE client.
 */

#include <QQuickItem>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include "TransactionListView.h"
#include "TransactionListModel.h"
using namespace dev::mix;

TransactionListView::TransactionListView(AppContext* _context): Extension(_context, ExtensionDisplayBehavior::RightView)
{
	m_model.reset(new TransactionListModel(this, _context));
	m_appEngine->rootContext()->setContextProperty("transactionListModel", m_model.get());
}

TransactionListView::~TransactionListView()
{
	//implementation is in cpp file so that all types deleted are complete
}

QString TransactionListView::contentUrl() const
{
	return QStringLiteral("qrc:/qml/TransactionList.qml");
}

QString TransactionListView::title() const
{
	return QApplication::tr("Transactions");
}

void TransactionListView::start() const
{
}