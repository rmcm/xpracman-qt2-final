--     $Id: mvac_label-crea.sql 2143 2004-02-24 20:53:49Z source $
-- 
--   Copyright 2004 X=X Computer Software Trust
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

DROP TABLE mvac_label;
CREATE TABLE mvac_label (
  perms integer PRIMARY KEY,
  label text
);

INSERT INTO mvac_label VALUES ( 1, 'guest' );
INSERT INTO mvac_label VALUES ( 2, 'user' );
INSERT INTO mvac_label VALUES ( 4, 'supervisor' );
INSERT INTO mvac_label VALUES ( 8, 'medical' );
INSERT INTO mvac_label VALUES ( 16, 'office' );
INSERT INTO mvac_label VALUES ( 32, 'office-admin' );
INSERT INTO mvac_label VALUES ( 64, 'local-admin' );
INSERT INTO mvac_label VALUES ( 127, 'support-admin' );

grant select on mvac_label to public;

-- update history;
\set mttb_name '\'mvac_label\''
\set mttb_rcs_header '\'$Id: mvac_label-crea.sql 2143 2004-02-24 20:53:49Z source $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

