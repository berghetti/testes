#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/smp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChatGPT");
MODULE_DESCRIPTION("Kernel module for user-level access to IPI");

static struct kobject *ipi_kobj;
static int target_cpu;

static ssize_t ipi_target_cpu_show(struct kobject *kobj,
                                   struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", target_cpu);
}

static ssize_t ipi_target_cpu_store(struct kobject *kobj,
                                    struct kobj_attribute *attr,
                                    const char *buf, size_t count)
{
    sscanf(buf, "%du", &target_cpu);
    return count;
}

static ssize_t ipi_send(struct file *file, const char __user *buf,
                        size_t count, loff_t *ppos)
{
    smp_call_function_single(target_cpu, smp_send_reschedule, NULL, 0);
    return count;
}

static struct kobj_attribute target_cpu_attr =
        __ATTR(target_cpu, 0666, ipi_target_cpu_show, ipi_target_cpu_store);

static struct file_operations ipi_fops = {
        .owner = THIS_MODULE,
        .write = ipi_send,
};

static int __init ipi_init(void)
{
    int ret;

    ipi_kobj = kobject_create_and_add("ipi", NULL);
    if (!ipi_kobj) {
        pr_err("Failed to create IPI kobject\n");
        return -ENOMEM;
    }

    ret = sysfs_create_file(ipi_kobj, &target_cpu_attr.attr);
    if (ret) {
        pr_err("Failed to create target_cpu sysfs file\n");
        kobject_put(ipi_kobj);
        return ret;
    }

    ret = sysfs_create_file(ipi_kobj, &dev_attr_ipi.attr);
    if (ret) {
        pr_err("Failed to create ipi sysfs file\n");
        sysfs_remove_file(ipi_kobj, &target_cpu_attr.attr);
        kobject_put(ipi_kobj);
        return ret;
    }

    return 0;
}

static void __exit ipi_exit(void)
{
    sysfs_remove_file(ipi_kobj, &dev_attr_ipi.attr);
    sysfs_remove_file(ipi_kobj, &target_cpu_attr.attr);
    kobject_put(ipi_kobj);
}

module_init(ipi_init);
module_exit(ipi_exit);
