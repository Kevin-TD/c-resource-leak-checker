#include "CVE-2022-3544.h"

/**
 * struct damon_operations - Monitoring operations for given use cases.
 *
 * @id:				Identifier of this operations set.
 * @init:			Initialize operations-related data structures.
 * @update:			Update operations-related data structures.
 * @prepare_access_checks:	Prepare next access check of target regions.
 * @check_accesses:		Check the accesses to target regions.
 * @reset_aggregated:		Reset aggregated accesses monitoring results.
 * @get_scheme_score:		Get the score of a region for a scheme.
 * @apply_scheme:		Apply a DAMON-based operation scheme.
 * @target_valid:		Determine if the target is valid.
 * @cleanup:			Clean up the context.
 *
 * DAMON can be extended for various address spaces and usages.  For this,
 * users should register the low level operations for their target address
 * space and usecase via the &damon_ctx.ops.  Then, the monitoring thread
 * (&damon_ctx.kdamond) calls @init and @prepare_access_checks before starting
 * the monitoring, @update after each &damon_attrs.ops_update_interval, and
 * @check_accesses, @target_valid and @prepare_access_checks after each
 * &damon_attrs.sample_interval.  Finally, @reset_aggregated is called after
 * each &damon_attrs.aggr_interval.
 *
 * Each &struct damon_operations instance having valid @id can be registered
 * via damon_register_ops() and selected by damon_select_ops() later.
 * @init should initialize operations-related data structures.  For example,
 * this could be used to construct proper monitoring target regions and link
 * those to @damon_ctx.adaptive_targets.
 * @update should update the operations-related data structures.  For example,
 * this could be used to update monitoring target regions for current status.
 * @prepare_access_checks should manipulate the monitoring regions to be
 * prepared for the next access check.
 * @check_accesses should check the accesses to each region that made after the
 * last preparation and update the number of observed accesses of each region.
 * It should also return max number of observed accesses that made as a result
 * of its update.  The value will be used for regions adjustment threshold.
 * @reset_aggregated should reset the access monitoring results that aggregated
 * by @check_accesses.
 * @get_scheme_score should return the priority score of a region for a scheme
 * as an integer in [0, &DAMOS_MAX_SCORE].
 * @apply_scheme is called from @kdamond when a region for user provided
 * DAMON-based operation scheme is found.  It should apply the scheme's action
 * to the region and return bytes of the region that the action is successfully
 * applied.
 * @target_valid should check whether the target is still valid for the
 * monitoring.
 * @cleanup is called from @kdamond just before its termination.
 */
struct damon_operations {
	enum damon_ops_id id;
	void (*init)(struct damon_ctx *context);
	void (*update)(struct damon_ctx *context);
	void (*prepare_access_checks)(struct damon_ctx *context);
	unsigned int (*check_accesses)(struct damon_ctx *context);
	void (*reset_aggregated)(struct damon_ctx *context);
	int (*get_scheme_score)(struct damon_ctx *context,
			struct damon_target *t, struct damon_region *r,
			struct damos *scheme);
	unsigned long (*apply_scheme)(struct damon_ctx *context,
			struct damon_target *t, struct damon_region *r,
			struct damos *scheme);
	bool (*target_valid)(struct damon_target *t);
	void (*cleanup)(struct damon_ctx *context);
};
