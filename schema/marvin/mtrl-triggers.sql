-- -*- sql -*-
 
--    $Id: mtrl-triggers.sql 771 2001-09-01 03:40:05Z source $

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
-- PL function to set defaults if null
--
drop function mtrl_tr_before();
create function mtrl_tr_before()
returns opaque
as 'DECLARE
    rec_mtcl mtcl%ROWTYPE;
    rec_mtat mtat%ROWTYPE;

    BEGIN

    -- check/set mtrl_master_class

    select *
    into   rec_mtcl
    from   mtcl
    where  mtcl_name = new.mtrl_master_class;

    if ( not found ) then
      if ( new.mtrl_master_class = ''-'' ) then
        RAISE NOTICE ''inserting master class record for <%>'', new.mtrl_master_class;
        insert into mtcl(mtcl_name) values(new.mtrl_master_class);
      else
        RAISE EXCEPTION ''The TABLE <%> does not exist in the MTCL table'', new.mtrl_master_class;
        return null;
      end if;
    end if;

    -- check/set mtrl_other_class

    select *
    into   rec_mtcl
    from   mtcl
    where  mtcl_name = new.mtrl_other_class;

    if ( not found ) then
      if ( new.mtrl_other_class = ''-'' ) then
        RAISE NOTICE ''inserting other class record for <%>'', new.mtrl_other_class;
        insert into mtcl(mtcl_name) values(new.mtrl_other_class);
      else
        RAISE EXCEPTION ''The TABLE <%> does not exist in the MTCL table'', new.mtrl_other_class;
        return null;
      end if;
    end if;

    -- check/set mtrl_key

    select *
    into   rec_mtat
    from   mtat
    where  mtat_name = new.mtrl_key;

    if ( not found ) then
      if ( new.mtrl_key = ''-'' ) then
        RAISE NOTICE ''inserting attribute record for <%>'', new.mtrl_key;
        insert into mtat(mtat_name, mtat_class_name) values(new.mtrl_key, new.mtrl_master_class);
      else
        RAISE EXCEPTION ''The ATTRIBUTE <%> does not exist in the MTAT table'', new.mtrl_key;
        return null;
      end if;
    end if;


    -- check/set mtrl_join

    select *
    into   rec_mtat
    from   mtat
    where  mtat_name = new.mtrl_join;

    if ( not found ) then
      if ( new.mtrl_join = ''-'' ) then
        RAISE NOTICE ''inserting attribute record for <%>'', new.mtrl_join;
        insert into mtat(mtat_name, mtat_class_name) values(new.mtrl_join, new.mtrl_other_class);
      else
        RAISE EXCEPTION ''The ATTRIBUTE <%> does not exist in the MTAT table'', new.mtrl_join;
        return null;
      end if;
    end if;


    return new;

    END;'
    LANGUAGE 'plpgsql';

drop trigger mtrl_tr_before on mtrl;
create trigger mtrl_tr_before before insert or update
    on mtrl for each row
    execute procedure mtrl_tr_before();
