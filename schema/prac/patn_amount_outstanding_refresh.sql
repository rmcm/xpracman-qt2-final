--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
-- 
-- 
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--  
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--  
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--  
--  Report problems and direct all questions to:
--  
--      Rex McMaster, rmcm@compsoft.com.au
--  
-- 

begin;
select disable_triggers();
UPDATE  patn
set     patn_amount_outstanding = (
                select  sum(invc_balance(invc__sequence))
                from    invc
                where   invc_patn__sequence = patn.patn__sequence );
UPDATE  dbtr
set     dbtr_amount_outstanding = (
                select  sum(invc_balance(invc__sequence))
                from    invc
                where   invc_dbtr_code = dbtr.dbtr_code );

select enable_triggers();
select 'transaction in progress - commit or abort' as notice;

