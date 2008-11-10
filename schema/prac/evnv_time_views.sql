--     $Id$
-- 
--   Copyright 2006 X=X Computer Software Trust
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



CREATE VIEW evnv_am AS
  SELECT  *
  FROM    evnv
  WHERE   extract('hour' from evnv.evnv_starttime) < 13;

grant select, insert, update, delete on evnv_am to public;

CREATE VIEW evnv_pm AS
  SELECT  *
  FROM    evnv
  WHERE   extract('hour' from evnv.evnv_starttime) >= 13;

grant select, insert, update, delete on evnv_pm to public;

