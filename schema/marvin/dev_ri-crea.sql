-- from postgresql RI site.
drop view dev_ri;
CREATE VIEW dev_ri
AS 
SELECT t.oid as trigoid,
    t.tgname as trigname,
    c.relname as trig_tbl,
    t.tgtype as trig_type,
    t.tgfoid, 
    f.proname as trigfunc,
    t.tgenabled,
    t.tgconstrname,
    c2.relname as const_tbl,
    t.tgdeferrable,
    t.tginitdeferred
    FROM pg_trigger t,
    pg_class c, 
    pg_class c2, 
    pg_proc f 
    WHERE t.tgrelid=c.oid 
    AND t.tgconstrrelid=c2.oid 
    AND tgfoid=f.oid
    AND tgname ~ '^RI_'
    ORDER BY t.oid;
