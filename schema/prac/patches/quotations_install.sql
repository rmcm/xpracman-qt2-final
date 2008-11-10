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
-- Install Quotations Module

\i schema/prac/dbtr-functions.sql
\i schema/prac/fept-crea.sql
\i schema/prac/qute-crea.sql
\i schema/prac/quit-crea.sql
\i schema/prac/qutv-crea.sql
\i schema/prac/quiv-crea.sql
\i schema/prac/forms/quotation.sql
\i schema/prac/forms/quotation_base.sql

update fmdt set fmdt_form_code = 'invoice-svpv' where fmdt_text ~* 'cheques payable' and fmdt_form_code = 'lhinvc';
