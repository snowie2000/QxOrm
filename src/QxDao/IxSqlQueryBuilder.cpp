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

#include <QxPrecompiled.h>

#include <QxDao/IxSqlQueryBuilder.h>

#include <QxMemLeak/mem_leak.h>

namespace qx {

IxSqlQueryBuilder::~IxSqlQueryBuilder() { ; }

void IxSqlQueryBuilder::displaySqlQuery(int time_ms /* = -1 */) const
{
   if (time_ms < 0)  { qDebug("[QxOrm] sql query : %s", qPrintable(m_sSqlQuery)); }
   else              { qDebug("[QxOrm] sql query (%d ms) : %s", time_ms, qPrintable(m_sSqlQuery)); }
}

void IxSqlQueryBuilder::initIdX()
{
   if (m_pIdX) { return; }
   if (! m_bCartesianProduct || ! m_lstSqlRelationPtr) { qAssert(false); return; }

   m_pIdX.reset(new type_lst_ptr_by_id());
   type_ptr_by_id_ptr pItem;
   pItem.reset(new type_ptr_by_id());
   m_pIdX->append(pItem); // index 0 -> owner

   for (long l = 0; l < m_lstSqlRelationPtr->count(); ++l)
   {
      pItem.reset(new type_ptr_by_id());
      m_pIdX->append(pItem); // index 1 2 3 4 etc... -> relation
   }
}

bool IxSqlQueryBuilder::insertIdX(long lIndex, const QVariant & idOwner, const QVariant & idData, void * ptr)
{
   this->initIdX();
   QString sIdOwner = idOwner.toString(); QString sIdData = idData.toString();
   if (! m_pIdX || sIdOwner.isEmpty() || sIdData.isEmpty()) { qAssert(false); return false; }
   if ((lIndex < 0) || (lIndex >= m_pIdX->count())) { qAssert(false); return false; }

   type_id idX(sIdOwner, sIdData);
   type_ptr_by_id_ptr pHash = m_pIdX->at(lIndex);
   if (! ptr || ! pHash || pHash->contains(idX)) { qAssert(false); return false; }
   pHash->insert(idX, ptr);

   return true;
}

void * IxSqlQueryBuilder::existIdX(long lIndex, const QVariant & idOwner, const QVariant & idData)
{
   this->initIdX();
   QString sIdOwner = idOwner.toString(); QString sIdData = idData.toString();
   if (! m_pIdX || sIdOwner.isEmpty() || sIdData.isEmpty()) { qAssert(false); return NULL; }
   if ((lIndex < 0) || (lIndex >= m_pIdX->count())) { qAssert(false); return NULL; }

   type_id idX(sIdOwner, sIdData);
   type_ptr_by_id_ptr pHash = m_pIdX->at(lIndex);
   if (! pHash || ! pHash->contains(idX)) { return NULL; }

   return pHash->value(idX);
}

} // namespace qx