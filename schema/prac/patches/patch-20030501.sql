--     $Id$
-- 
--   Copyright 2003 X=X Computer Software Trust
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


-- Patch file

\i dumpdir/prac/forms/list-svpv-form.sql

\i schema/prac/svsm-crea.sql
\i schema/prac/svsm-functions.sql
\i meta-data/prac/svsm-meta.sql
\i dumpdir/prac/forms/list-svsm-form.sql

\i schema/prac/crsm-crea.sql
\i schema/prac/crsm-functions.sql
\i meta-data/prac/crsm-meta.sql
\i dumpdir/prac/forms/list-crsm-form.sql

-- register the patch
insert into mtpt(mtpt_name, mtpt_rcs_header) values('patch-20030501.sql', '$Id$');
