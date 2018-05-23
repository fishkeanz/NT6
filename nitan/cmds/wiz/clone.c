// clone.c

inherit F_CLEAN_UP;

int main(object me, string file)
{
        object obj;
        string err, msg;

        if (! SECURITY_D->valid_grant(me, "(wizard)"))
                return 0;

        seteuid(getuid());

        if( !file)file=query("cwf", me);
        if (! file)
                return notify_fail("��Ҫ����ʲ�������\n");

        file=resolve_path(query("cwd", me),file);
        if (sscanf(file, "%*s.c") != 1) file += ".c";
        set("cwf", file, me);

        if (file_size(file) < 0)
                return notify_fail("û���������(" + file + ")��\n");

        if (! SECURITY_D->valid_read(file, me, "clone"))
        {
                write("��û��Ȩ�޲����������\n");
                return 1;
        }

        if (! find_object(file))
        {
                err = catch(call_other(file, "???"));
                if (err)
                {
                        write("����ʧ�ܣ�" + err + "\n");
                        return 1;
                }
        }

        if (! me->is_admin() && query("id", me) !="luoyun")
        {
                switch (SECURITY_D->query_site_privilege("clone"))
                {
                case "gift":
                        if (! sscanf(file, "/clone/special/%*s") &&
                            ! sscanf(file, "/clone/gift/%*s"))
                                return notify_fail("�㲻�ܸ��������Ʒ��\n");
                        break;

                case "all":
                        break;

                default:
                        return notify_fail("�㲻�ܸ�����Ʒ��\n");
                }

                if (! me->is_admin())
                        message_system(sprintf("%s(%s)��������Ʒ��%s(%s)��\n",
                                               me->name(1),query("id", me),
                                               filter_color(file->name(1)),query("id", get_object(file))));
        }

        seteuid(geteuid(me));

        err = catch(obj = new(file));
        if (err)
        {
                write("����ʧ�ܣ�" + err + "\n");
                return 1;
        }

        if (! objectp(obj))
        {
                write("���޷����Ƹ���Ʒ��\n");
                return 1;
        }

        msg = "ֻ��$N�������һָ�������$n��\n";

        log_file("static/clone", sprintf("%s %-9s clone %s\n",
                                         log_time(), geteuid(me),
                                         base_name(obj)));
        if( !obj->is_character() && !query("no_get", obj) && obj->move(me) )
        {
                write(query("name", obj)+"���Ƴɹ������������Ʒ����\n");
                message_vision(msg + "\n", me, obj);
                return 1;
        }
        if (obj->move(environment(me)))
        {
                write(query("name", obj)+"���Ƴɹ�������������䡣\n");
                message_vision(msg + "\n", me, obj);
                return 1;
        }

        destruct(obj);
        return notify_fail("�޷����Ʋ����ƶ������(" + file + ")��\n");
}

int help(object me)
{
        write(@HELP
ָ���ʽ : clone <����>

���ô�ָ��ɸ����κ����ƶ�֮���(������)��

�������ڿ��Ա���Ȩʹ�õ���Ϣ������gift��all��
HELP );
        return 1;
}