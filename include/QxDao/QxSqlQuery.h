/****************************************************************************
**
** http://www.qxorm.com/
** http://sourceforge.net/projects/qxorm/
** Original file by Lionel Marty
**
** This file is part of the QxOrm library
**
** This software is provided 'as-is', without any express or implied
** warranty. In no event will the authors be held liable for any
** damages arising from the use of this software.
**
** GNU Lesser General Public License Usage
** This file must be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file 'license.lgpl.txt' included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact :
** contact@qxorm.com
**
****************************************************************************/

#ifndef _QX_SQL_QUERY_H_
#define _QX_SQL_QUERY_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file QxSqlQuery.h
 * \author Lionel Marty
 * \ingroup QxDao
 * \brief Define a user SQL query added to default SQL query builded by QxOrm library, and used by qx::dao::xxx functions to filter elements fetched from database
 */

#include <QtSql/qsqlquery.h>

#include <QxCollection/QxCollection.h>

namespace qx {

/*!
 * \ingroup QxDao
 * \brief qx::QxSqlQuery : define a user SQL query added to default SQL query builded by QxOrm library, and used by qx::dao::xxx functions to filter elements fetched from database
 *
 * Quick sample using qx::QxSqlQuery class :
 * \code
// Create a query to fetch only female author : 'author_id_2' and 'author_id_3'
qx::QxSqlQuery query("WHERE author.sex = :sex");
query.bind(":sex", author::female);

list_author list_of_female_author;
daoError = qx::dao::fetch_by_query(query, list_of_female_author);
qAssert(list_of_female_author.count() == 2);
 * \endcode
 */
class QX_DLL_EXPORT QxSqlQuery
{

protected:

   QString m_sQuery;                               //!< Query SQL with place-holder
   QxCollection<QString, QVariant> m_lstValue;     //!< Bind value in this array

public:

   QxSqlQuery() { ; }
   QxSqlQuery(const QString & sQuery) : m_sQuery(sQuery) { ; }
   QxSqlQuery(const char * sQuery) : m_sQuery(sQuery) { ; }
   virtual ~QxSqlQuery() { ; }

   QString query() const   { return m_sQuery; }
   bool isEmpty() const    { return m_sQuery.isEmpty(); }

   QxSqlQuery & query(const QString & sQuery);

   QxSqlQuery & bind(const QVariant & vValue);
   QxSqlQuery & bind(const QString & sKey, const QVariant & vValue);

   void resolve(QSqlQuery & query) const;

};

} // namespace qx

#endif // _QX_SQL_QUERY_H_