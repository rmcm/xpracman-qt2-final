-- -*- sql -*-
 
--    $Id: mtrl-addconstraints.sql 1288 2002-04-30 00:01:29Z source $

--  Copyright 1996 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 

 
-- Revision Log
  
--
-- Foreign Key constraints
--

alter table mtrl
        add constraint mtrl_fk_master_mtcl
            foreign key (mtrl_master_class)
            references mtcl
            on update cascade
            on delete cascade
            initially deferred;

alter table mtrl
        add constraint mtrl_fk_other_mtcl
            foreign key (mtrl_other_class)
            references mtcl
            on update cascade
            on delete cascade
            initially deferred;
