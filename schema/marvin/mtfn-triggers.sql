-- -*- sql -*-
 
--    $Id: mtfn-triggers.sql 599 2001-07-03 21:51:27Z source $

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
 
--     Rex McMaster, rmcm@ariel.ucs.unimelb.edu.au
 

 
-- Revision Log
  
--
-- PL function to set defaults if null
--
drop function mtfn_tr_before();
create function mtfn_tr_before()
returns opaque
as 'DECLARE
    rec_mtcl mtcl%ROWTYPE;
    rec_mtat mtat%ROWTYPE;

    BEGIN

    -- check/set mtfn_master_class

    select *
    into   rec_mtcl
    from   mtcl
    where  mtcl_name = new.mtfn_master_class;

    if ( not found ) then
      if ( new.mtfn_master_class = ''-'' ) then
        RAISE NOTICE ''inserting master class record for <%>'', new.mtfn_master_class;
        insert into mtcl(mtcl_name) values(new.mtfn_master_class);
      else
        RAISE EXCEPTION ''The TABLE <%> does not exist in the MTCL table'', new.mtfn_master_class;
        return null;
      end if;
    end if;

    -- check/set mtfn_other_class

    select *
    into   rec_mtcl
    from   mtcl
    where  mtcl_name = new.mtfn_other_class;

    if ( not found ) then
      if ( new.mtfn_other_class = ''-'' ) then
        RAISE NOTICE ''inserting other class record for <%>'', new.mtfn_other_class;
        insert into mtcl(mtcl_name) values(new.mtfn_other_class);
      else
        RAISE EXCEPTION ''The TABLE <%> does not exist in the MTCL table'', new.mtfn_other_class;
        return null;
      end if;
    end if;

    -- check/set mtfn_key

    select *
    into   rec_mtat
    from   mtat
    where  mtat_name = new.mtfn_key;

    if ( not found ) then
      if ( new.mtfn_key = ''-'' ) then
        RAISE NOTICE ''inserting attribute record for <%>'', new.mtfn_key;
        insert into mtat(mtat_name, mtat_class_name) values(new.mtfn_key, new.mtfn_master_class);
      else
        RAISE EXCEPTION ''The ATTRIBUTE <%> does not exist in the MTAT table'', new.mtfn_key;
        return null;
      end if;
    end if;


    -- check/set mtfn_join

    select *
    into   rec_mtat
    from   mtat
    where  mtat_name = new.mtfn_join;

    if ( not found ) then
      if ( new.mtfn_join = ''-'' ) then
        RAISE NOTICE ''inserting attribute record for <%>'', new.mtfn_join;
        insert into mtat(mtat_name, mtat_class_name) values(new.mtfn_join, new.mtfn_other_class);
      else
        RAISE EXCEPTION ''The ATTRIBUTE <%> does not exist in the MTAT table'', new.mtfn_join;
        return null;
      end if;
    end if;

    -- check/set mtfn_view

    select *
    into   rec_mtat
    from   mtat
    where  mtat_name = new.mtfn_view;

    if ( not found ) then
      if ( new.mtfn_view = ''-'' ) then
        RAISE NOTICE ''inserting attribute record for <%>'', new.mtfn_view;
        insert into mtat(mtat_name, mtat_class_name) values(new.mtfn_view, new.mtfn_other_class);
      else
        RAISE EXCEPTION ''The ATTRIBUTE <%> does not exist in the MTAT table'', new.mtfn_view;
        return null;
      end if;
    end if;


    return new;

    END;'
    LANGUAGE 'plpgsql';

drop trigger mtfn_tr_before on mtfn;
create trigger mtfn_tr_before before insert or update
    on mtfn for each row
    execute procedure mtfn_tr_before();
