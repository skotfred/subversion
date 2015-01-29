#define DIFF_REVNUM_NONEXISTENT ((svn_revnum_t) -100)
/* Calculate the repository relative path of DIFF_RELPATH, using
 * SESSION_RELPATH and WC_CTX, and return the result in *REPOS_RELPATH.
                   const char *session_relpath,
  if (! session_relpath
      if (!session_relpath
  *repos_relpath = svn_relpath_join(session_relpath, diff_relpath,
                                    result_pool);
       simply join dwi->orig_path_1 with path, ditto for
    new_path2 = apr_psprintf(result_pool, "%s\t(%s)", new_path, new_path2);
  if (revnum >= 0)
  else if (revnum == DIFF_REVNUM_NONEXISTENT)
    label = apr_psprintf(pool, _("%s\t(nonexistent)"), path);
  else /* SVN_INVALID_REVNUM */
                   const char *ra_session_relpath,
                   svn_cancel_func_t cancel_func,
                   void *cancel_baton,
                                 ra_session_relpath, wc_ctx, anchor,
                                 ra_session_relpath, wc_ctx, anchor,
            TRUE /* pretty_print_mergeinfo */,
            -1 /* context_size */,
            cancel_func, cancel_baton, scratch_pool));
/* State provided by the diff drivers; used by the diff writer */
typedef struct diff_driver_info_t
{
  /* The anchor to prefix before wc paths */
  const char *anchor;
   /* Relative path of ra session from repos_root_url */
  const char *session_relpath;
  /* The original targets passed to the diff command.  We may need
     these to construct distinctive diff labels when comparing the
     same relative path in the same revision, under different anchors
     (for example, when comparing a trunk against a branch). */
  const char *orig_path_1;
  const char *orig_path_2;
} diff_driver_info_t;


/* Diff writer state */
typedef struct diff_writer_info_t
{
  svn_boolean_t show_copies_as_adds;
  svn_cancel_func_t cancel_func;
  void *cancel_baton;
  struct diff_driver_info_t ddi;
} diff_writer_info_t;
                   diff_writer_info_t *dwi,
  if (dwi->ignore_properties)
      /* We're using the revnums from the dwi since there's
                                 dwi->ddi.anchor,
                                 dwi->ddi.orig_path_1,
                                 dwi->ddi.orig_path_2,
                                 dwi->header_encoding,
                                 dwi->outstream,
                                 dwi->relative_to_dir,
                                 dwi->use_git_diff_format,
                                 dwi->ddi.session_relpath,
                                 dwi->cancel_func,
                                 dwi->cancel_baton,
                                 dwi->wc_ctx,
                     diff_writer_info_t *dwi,
  const char *rel_to_dir = dwi->relative_to_dir;
  svn_stream_t *outstream = dwi->outstream;
  const char *path1 = dwi->ddi.orig_path_1;
  const char *path2 = dwi->ddi.orig_path_2;
  if (dwi->properties_only)
                                       rel_to_dir, dwi->ddi.anchor,
  if (! dwi->force_binary && (mt1_binary || mt2_binary))
               dwi->header_encoding, scratch_pool,
      if (dwi->use_git_diff_format)
        {
          svn_stream_t *left_stream;
          svn_stream_t *right_stream;
          const char *repos_relpath1;
          const char *repos_relpath2;

          SVN_ERR(make_repos_relpath(&repos_relpath1, diff_relpath,
                                      dwi->ddi.orig_path_1,
                                      dwi->ddi.session_relpath,
                                      dwi->wc_ctx,
                                      dwi->ddi.anchor,
                                      scratch_pool, scratch_pool));
          SVN_ERR(make_repos_relpath(&repos_relpath2, diff_relpath,
                                      dwi->ddi.orig_path_2,
                                      dwi->ddi.session_relpath,
                                      dwi->wc_ctx,
                                      dwi->ddi.anchor,
                                      scratch_pool, scratch_pool));
          SVN_ERR(print_git_diff_header(outstream, &label1, &label2,
                                        operation,
                                        repos_relpath1, repos_relpath2,
                                        rev1, rev2,
                                        copyfrom_path,
                                        copyfrom_rev,
                                        dwi->header_encoding,
                                        scratch_pool));

          SVN_ERR(svn_stream_open_readonly(&left_stream, tmpfile1,
                                           scratch_pool, scratch_pool));
          SVN_ERR(svn_stream_open_readonly(&right_stream, tmpfile2,
                                           scratch_pool, scratch_pool));
          SVN_ERR(svn_diff_output_binary(outstream,
                                         left_stream, right_stream,
                                         dwi->cancel_func, dwi->cancel_baton,
                                         scratch_pool));
        }
      else
          SVN_ERR(svn_stream_printf_from_utf8(outstream,
                   dwi->header_encoding, scratch_pool,
                   _("Cannot display: file marked as a binary type.%s"),
                   APR_EOL_STR));

          if (mt1_binary && !mt2_binary)
                     dwi->header_encoding, scratch_pool,
                     "svn:mime-type = %s" APR_EOL_STR, mimetype1));
          else if (mt2_binary && !mt1_binary)
                     dwi->header_encoding, scratch_pool,
                     "svn:mime-type = %s" APR_EOL_STR, mimetype2));
          else if (mt1_binary && mt2_binary)
            {
              if (strcmp(mimetype1, mimetype2) == 0)
                SVN_ERR(svn_stream_printf_from_utf8(outstream,
                         dwi->header_encoding, scratch_pool,
                         "svn:mime-type = %s" APR_EOL_STR,
                         mimetype1));
              else
                SVN_ERR(svn_stream_printf_from_utf8(outstream,
                         dwi->header_encoding, scratch_pool,
                         "svn:mime-type = (%s, %s)" APR_EOL_STR,
                         mimetype1, mimetype2));
            }
  if (dwi->diff_cmd)
      svn_stream_t *errstream = dwi->errstream;
      int exitcode;
               dwi->header_encoding, scratch_pool,
                               dwi->options.for_external.argv,
                               dwi->options.for_external.argc,
                               dwi->diff_cmd, scratch_pool));
      /* If we have printed a diff for this path, mark it as visited. */
      if (exitcode == 1)
        *wrote_header = TRUE;
                                   dwi->options.for_internal,
          || dwi->use_git_diff_format
                   dwi->header_encoding, scratch_pool,
          if (dwi->use_git_diff_format)
                                         dwi->ddi.orig_path_1,
                                         dwi->ddi.session_relpath,
                                         dwi->wc_ctx,
                                         dwi->ddi.anchor,
                                         dwi->ddi.orig_path_2,
                                         dwi->ddi.session_relpath,
                                         dwi->wc_ctx,
                                         dwi->ddi.anchor,
                                            dwi->header_encoding,
            SVN_ERR(svn_diff_file_output_unified4(outstream, diff,
                     dwi->header_encoding, rel_to_dir,
                     dwi->options.for_internal->show_c_function,
                     dwi->options.for_internal->context_size,
                     dwi->cancel_func, dwi->cancel_baton,
          /* If we have printed a diff for this path, mark it as visited. */
          if (dwi->use_git_diff_format || svn_diff_contains_diffs(diff))
            *wrote_header = TRUE;
/* An svn_diff_tree_processor_t callback. */
diff_file_changed(const char *relpath,
                  const svn_diff_source_t *left_source,
                  const svn_diff_source_t *right_source,
                  const char *left_file,
                  const char *right_file,
                  /*const*/ apr_hash_t *left_props,
                  /*const*/ apr_hash_t *right_props,
                  svn_boolean_t file_modified,
                  void *file_baton,
                  const struct svn_diff_tree_processor_t *processor,
  diff_writer_info_t *dwi = processor->baton;
  if (file_modified)
    SVN_ERR(diff_content_changed(&wrote_header, relpath,
                                 left_file, right_file,
                                 left_source->revision,
                                 right_source->revision,
                                 svn_prop_get_value(left_props,
                                                    SVN_PROP_MIME_TYPE),
                                 svn_prop_get_value(right_props,
                                                    SVN_PROP_MIME_TYPE),
                                 SVN_INVALID_REVNUM, dwi,
    SVN_ERR(diff_props_changed(relpath,
                               left_source->revision,
                               right_source->revision, prop_changes,
                               left_props, !wrote_header,
                               dwi, scratch_pool));
/* An svn_diff_tree_processor_t callback. */
diff_file_added(const char *relpath,
                const svn_diff_source_t *copyfrom_source,
                const svn_diff_source_t *right_source,
                const char *copyfrom_file,
                const char *right_file,
                /*const*/ apr_hash_t *copyfrom_props,
                /*const*/ apr_hash_t *right_props,
                void *file_baton,
                const struct svn_diff_tree_processor_t *processor,
  diff_writer_info_t *dwi = processor->baton;
  const char *left_file;
  apr_hash_t *left_props;
  apr_array_header_t *prop_changes;
  if (copyfrom_source && !dwi->show_copies_as_adds)
      left_file = copyfrom_file;
      left_props = copyfrom_props ? copyfrom_props : apr_hash_make(scratch_pool);
  else
      if (!dwi->empty_file)
        SVN_ERR(svn_io_open_unique_file3(NULL, &dwi->empty_file,
                                         dwi->pool, scratch_pool));
      left_file = dwi->empty_file;
      left_props = apr_hash_make(scratch_pool);
      copyfrom_source = NULL;
      copyfrom_file = NULL;
  SVN_ERR(svn_prop_diffs(&prop_changes, right_props, left_props, scratch_pool));

  if (dwi->no_diff_added)
      const char *index_path = relpath;
      if (dwi->ddi.anchor)
        index_path = svn_dirent_join(dwi->ddi.anchor, relpath,
      SVN_ERR(svn_stream_printf_from_utf8(dwi->outstream,
                dwi->header_encoding, scratch_pool,
  else if (copyfrom_source && right_file)
    SVN_ERR(diff_content_changed(&wrote_header, relpath,
                                 left_file, right_file,
                                 copyfrom_source->revision,
                                 right_source->revision,
                                 svn_prop_get_value(left_props,
                                                    SVN_PROP_MIME_TYPE),
                                 svn_prop_get_value(right_props,
                                                    SVN_PROP_MIME_TYPE),
                                 copyfrom_source->repos_relpath,
                                 copyfrom_source->revision,
                                 dwi, scratch_pool));
  else if (right_file)
    SVN_ERR(diff_content_changed(&wrote_header, relpath,
                                 left_file, right_file,
                                 DIFF_REVNUM_NONEXISTENT,
                                 right_source->revision,
                                 svn_prop_get_value(left_props,
                                                    SVN_PROP_MIME_TYPE),
                                 svn_prop_get_value(right_props,
                                                    SVN_PROP_MIME_TYPE),
                                 dwi, scratch_pool));
    SVN_ERR(diff_props_changed(relpath,
                               copyfrom_source ? copyfrom_source->revision
                                               : DIFF_REVNUM_NONEXISTENT,
                               right_source->revision,
                               prop_changes,
                               left_props, ! wrote_header,
                               dwi, scratch_pool));
/* An svn_diff_tree_processor_t callback. */
diff_file_deleted(const char *relpath,
                  const svn_diff_source_t *left_source,
                  const char *left_file,
                  /*const*/ apr_hash_t *left_props,
                  void *file_baton,
                  const struct svn_diff_tree_processor_t *processor,
  diff_writer_info_t *dwi = processor->baton;
  if (dwi->no_diff_deleted)
      const char *index_path = relpath;
      if (dwi->ddi.anchor)
        index_path = svn_dirent_join(dwi->ddi.anchor, relpath,
      SVN_ERR(svn_stream_printf_from_utf8(dwi->outstream,
                dwi->header_encoding, scratch_pool,

      if (!dwi->empty_file)
        SVN_ERR(svn_io_open_unique_file3(NULL, &dwi->empty_file,
                                         NULL, svn_io_file_del_on_pool_cleanup,
                                         dwi->pool, scratch_pool));

      if (left_file)
        SVN_ERR(diff_content_changed(&wrote_header, relpath,
                                     left_file, dwi->empty_file,
                                     left_source->revision,
                                     DIFF_REVNUM_NONEXISTENT,
                                     svn_prop_get_value(left_props,
                                                        SVN_PROP_MIME_TYPE),
                                     NULL,
                                     dwi,
      if (left_props && apr_hash_count(left_props))
        {
          apr_array_header_t *prop_changes;
          SVN_ERR(svn_prop_diffs(&prop_changes, apr_hash_make(scratch_pool),
                                 left_props, scratch_pool));
          SVN_ERR(diff_props_changed(relpath,
                                     left_source->revision,
                                     DIFF_REVNUM_NONEXISTENT,
                                     prop_changes,
                                     left_props, ! wrote_header,
                                     dwi, scratch_pool));
        }
    }
diff_dir_changed(const char *relpath,
                 const svn_diff_source_t *left_source,
                 const svn_diff_source_t *right_source,
                 /*const*/ apr_hash_t *left_props,
                 /*const*/ apr_hash_t *right_props,
                 const apr_array_header_t *prop_changes,
                 void *dir_baton,
                 const struct svn_diff_tree_processor_t *processor,
  diff_writer_info_t *dwi = processor->baton;

  SVN_ERR(diff_props_changed(relpath,
                             left_source->revision,
                             right_source->revision,
                             prop_changes,
                             left_props,
                             TRUE /* show_diff_header */,
                             dwi,
                             scratch_pool));
/* An svn_diff_tree_processor_t callback. */
diff_dir_added(const char *relpath,
               const svn_diff_source_t *copyfrom_source,
               const svn_diff_source_t *right_source,
               /*const*/ apr_hash_t *copyfrom_props,
               /*const*/ apr_hash_t *right_props,
               void *dir_baton,
               const struct svn_diff_tree_processor_t *processor,
               apr_pool_t *scratch_pool)
  diff_writer_info_t *dwi = processor->baton;
  apr_hash_t *left_props;
  apr_array_header_t *prop_changes;
  if (dwi->no_diff_added)
    return SVN_NO_ERROR;

  if (copyfrom_source && !dwi->show_copies_as_adds)
    {
      left_props = copyfrom_props ? copyfrom_props
                                  : apr_hash_make(scratch_pool);
    }
  else
    {
      left_props = apr_hash_make(scratch_pool);
      copyfrom_source = NULL;
    }

  SVN_ERR(svn_prop_diffs(&prop_changes, right_props, left_props,
                         scratch_pool));

  return svn_error_trace(diff_props_changed(relpath,
                                            copyfrom_source ? copyfrom_source->revision
                                                            : DIFF_REVNUM_NONEXISTENT,
                                            right_source->revision,
                                            prop_changes,
                                            left_props,
                                            TRUE /* show_diff_header */,
                                            dwi,
                                            scratch_pool));
/* An svn_diff_tree_processor_t callback. */
diff_dir_deleted(const char *relpath,
                 const svn_diff_source_t *left_source,
                 /*const*/ apr_hash_t *left_props,
                 void *dir_baton,
                 const struct svn_diff_tree_processor_t *processor,
                 apr_pool_t *scratch_pool)
  diff_writer_info_t *dwi = processor->baton;
  apr_array_header_t *prop_changes;

  if (dwi->no_diff_deleted)
    return SVN_NO_ERROR;


  SVN_ERR(svn_prop_diffs(&prop_changes, apr_hash_make(scratch_pool),
                         left_props, scratch_pool));

  SVN_ERR(diff_props_changed(relpath,
                             left_source->revision,
                             DIFF_REVNUM_NONEXISTENT,
                             prop_changes,
                             left_props,
                             TRUE /* show_diff_header */,
                             dwi,
                             scratch_pool));
  const char *local_abspath1 = NULL;
  const char *local_abspath2 = NULL;
  svn_client__pathrev_t *resolved1;
  svn_client__pathrev_t *resolved2 = NULL;
  enum svn_opt_revision_kind peg_kind = peg_revision->kind;
      SVN_ERR(svn_dirent_get_absolute(&local_abspath2, path_or_url2, pool));
      SVN_ERR(svn_wc__node_get_url(url2, ctx->wc_ctx, local_abspath2,
      wri_abspath = local_abspath2;
    *url2 = apr_pstrdup(pool, path_or_url2);
      SVN_ERR(svn_dirent_get_absolute(&local_abspath1, path_or_url1, pool));
      wri_abspath = local_abspath1;
  if (peg_kind != svn_opt_revision_unspecified
      || path_or_url1 == path_or_url2
      || local_abspath2)
      svn_error_t *err;

      err = svn_client__resolve_rev_and_url(&resolved2,
                                            *ra_session, path_or_url2,
                                            peg_revision, revision2,
                                            ctx, pool);
      if (err)
          if (err->apr_err != SVN_ERR_CLIENT_UNRELATED_RESOURCES
              && err->apr_err != SVN_ERR_FS_NOT_FOUND)
            return svn_error_trace(err);

          svn_error_clear(err);
          resolved2 = NULL;
    }
  else
    resolved2 = NULL;

  if (peg_kind != svn_opt_revision_unspecified
      || path_or_url1 == path_or_url2
      || local_abspath1)
    {
      svn_error_t *err;

      err = svn_client__resolve_rev_and_url(&resolved1,
                                            *ra_session, path_or_url1,
                                            peg_revision, revision1,
                                            ctx, pool);
      if (err)
          if (err->apr_err != SVN_ERR_CLIENT_UNRELATED_RESOURCES
              && err->apr_err != SVN_ERR_FS_NOT_FOUND)
            return svn_error_trace(err);

          svn_error_clear(err);
          resolved1 = NULL;
    }
  else
    resolved1 = NULL;

  if (resolved1)
    {
      *url1 = resolved1->url;
      *rev1 = resolved1->rev;
    }
  else
    {
      /* It would be nice if we could just return an error when resolving a
         location fails... But in many such cases we prefer diffing against
         an not existing location to show adds od removes (see issue #4153) */

      if (resolved2
          && (peg_kind != svn_opt_revision_unspecified
              || path_or_url1 == path_or_url2))
        *url1 = resolved2->url;
      else if (! local_abspath1)
        *url1 = path_or_url1;
      else
        SVN_ERR(svn_wc__node_get_url(url1, ctx->wc_ctx, local_abspath1,
                                     pool, pool));

      SVN_ERR(svn_client__get_revision_number(rev1, NULL, ctx->wc_ctx,
                                              local_abspath1 /* may be NULL */,
                                              *ra_session, revision1, pool));
    }
  if (resolved2)
    {
      *url2 = resolved2->url;
      *rev2 = resolved2->rev;
    }
  else
    {
      /* It would be nice if we could just return an error when resolving a
         location fails... But in many such cases we prefer diffing against
         an not existing location to show adds od removes (see issue #4153) */

      if (resolved1
          && (peg_kind != svn_opt_revision_unspecified
              || path_or_url1 == path_or_url2))
        *url2 = resolved1->url;
      /* else keep url2 */

      SVN_ERR(svn_client__get_revision_number(rev2, NULL, ctx->wc_ctx,
                                              local_abspath2 /* may be NULL */,
                                              *ra_session, revision2, pool));
  SVN_ERR(svn_ra_reparent(*ra_session, *url2, pool));
      svn_node_kind_t ignored_kind;
      svn_error_t *err;



      /* We might not have the necessary rights to read the root now.
         (It is ok to pass a revision here where the node doesn't exist) */
      err = svn_ra_check_path(*ra_session, "", *rev1, &ignored_kind, pool);

      if (err && (err->apr_err == SVN_ERR_RA_DAV_FORBIDDEN
                  || err->apr_err == SVN_ERR_RA_NOT_AUTHORIZED))
        {
          svn_error_clear(err);

          /* Ok, lets undo the reparent...

             We can't report replacements this way, but at least we can
             report changes on the descendants */

          *anchor1 = svn_path_url_add_component2(*anchor1, *target1, pool);
          *anchor2 = svn_path_url_add_component2(*anchor2, *target2, pool);
          *target1 = "";
          *target2 = "";

          SVN_ERR(svn_ra_reparent(*ra_session, *anchor1, pool));
        }
      else
        SVN_ERR(err);
   comparisons between nodes in the working copy.
diff_wc_wc(const char **root_relpath,
           svn_boolean_t *root_is_dir,
           struct diff_driver_info_t *ddi,
           const char *path1,
           const svn_diff_tree_processor_t *diff_processor,
           apr_pool_t *result_pool,
           apr_pool_t *scratch_pool)
  SVN_ERR(svn_dirent_get_absolute(&abspath1, path1, scratch_pool));
  if (ddi)
      svn_node_kind_t kind;
      SVN_ERR(svn_wc_read_kind2(&kind, ctx->wc_ctx, abspath1,
                              TRUE, FALSE, scratch_pool));
      if (kind != svn_node_dir)
        ddi->anchor = svn_dirent_dirname(path1, scratch_pool);
      else
        ddi->anchor = path1;
    }
  SVN_ERR(svn_wc__diff7(root_relpath, root_is_dir,
                        ctx->wc_ctx, abspath1, depth,
                        ignore_ancestry, changelists,
                        diff_processor,
                        ctx->cancel_func, ctx->cancel_baton,
                        result_pool, scratch_pool));
diff_repos_repos(const char **root_relpath,
                 svn_boolean_t *root_is_dir,
                 struct diff_driver_info_t *ddi,
                 svn_boolean_t text_deltas,
                 const svn_diff_tree_processor_t *diff_processor,
                 svn_client_ctx_t *ctx,
                 apr_pool_t *result_pool,
                 apr_pool_t *scratch_pool)
  SVN_ERR(diff_prepare_repos_repos(&url1, &url2, &rev1, &rev2,
                                   scratch_pool));
  if (ddi)
    {
      /* Get actual URLs. */
      ddi->orig_path_1 = url1;
      ddi->orig_path_2 = url2;

      /* This should be moved to the diff writer
         - path_or_url are provided by the caller
         - target1 is available as *root_relpath
         - (kind1 != svn_node_dir || kind2 != svn_node_dir) = !*root_is_dir */

      if (!svn_path_is_url(path_or_url2))
        ddi->anchor = path_or_url2;
      else if (!svn_path_is_url(path_or_url1))
        ddi->anchor = path_or_url1;
      else
        ddi->anchor = NULL;
      if (*target1 && ddi->anchor
          && (kind1 != svn_node_dir || kind2 != svn_node_dir))
        ddi->anchor = svn_dirent_dirname(ddi->anchor, result_pool);
    }
                                                               NULL,
                                                               scratch_pool);
  if (root_relpath)
    *root_relpath = apr_pstrdup(result_pool, target1);
  else if ((kind1 != svn_node_file && kind2 != svn_node_file)
           && target1[0] != '\0')
    {
      diff_processor = svn_diff__tree_processor_filter_create(
                                        diff_processor, target1, scratch_pool);
    }
  SVN_ERR(svn_ra_dup_session(&extra_ra_session, ra_session, anchor1,
                             scratch_pool, scratch_pool));

  if (ddi)
    {
      const char *repos_root_url;
      const char *session_url;

      SVN_ERR(svn_ra_get_repos_root2(ra_session, &repos_root_url,
                                      scratch_pool));
      SVN_ERR(svn_ra_get_session_url(ra_session, &session_url,
                                      scratch_pool));

      ddi->session_relpath = svn_uri_skip_ancestor(repos_root_url,
                                                    session_url,
                                                    result_pool);
    }
                text_deltas,
                scratch_pool));
                          depth, ignore_ancestry, text_deltas,
                          url2, diff_editor, diff_edit_baton, scratch_pool));
                             scratch_pool));
  return svn_error_trace(
                  reporter->finish_report(reporter_baton, scratch_pool));
diff_repos_wc(const char **root_relpath,
              svn_boolean_t *root_is_dir,
              struct diff_driver_info_t *ddi,
              const char *path_or_url1,
              const svn_diff_tree_processor_t *diff_processor,
              apr_pool_t *result_pool,
  const char *anchor, *anchor_url, *target;
  svn_depth_t cf_depth;
  const char *copy_root_abspath;
  const char *target_url;
  svn_client__pathrev_t *loc1;
      SVN_ERR(svn_dirent_get_absolute(&abspath_or_url1, path_or_url1,
                                      scratch_pool));
  SVN_ERR(svn_dirent_get_absolute(&abspath2, path2, scratch_pool));
  /* Check if our diff target is a copied node. */
  SVN_ERR(svn_wc__node_get_origin(&is_copy,
                                  &cf_revision,
                                  &cf_repos_relpath,
                                  &cf_repos_root_url,
                                  NULL, &cf_depth,
                                  &copy_root_abspath,
                                  ctx->wc_ctx, abspath2,
                                  FALSE, scratch_pool, scratch_pool));
  SVN_ERR(svn_client__ra_session_from_path2(&ra_session, &loc1,
                                            path_or_url1, abspath2,
                                            peg_revision, revision1,
                                            ctx, scratch_pool));
  if (revision2->kind == svn_opt_revision_base || !is_copy)
      /* Convert path_or_url1 to a URL to feed to do_diff. */
      SVN_ERR(svn_wc_get_actual_target2(&anchor, &target, ctx->wc_ctx, path2,
                                        scratch_pool, scratch_pool));

      /* Handle the ugly case where target is ".." */
      if (*target && !svn_path_is_single_path_component(target))
        {
          anchor = svn_dirent_join(anchor, target, scratch_pool);
          target = "";
        }

      if (root_relpath)
        *root_relpath = apr_pstrdup(result_pool, target);
      if (root_is_dir)
        *root_is_dir = (*target == '\0');

      /* Fetch the URL of the anchor directory. */
      SVN_ERR(svn_dirent_get_absolute(&anchor_abspath, anchor, scratch_pool));
      SVN_ERR(svn_wc__node_get_url(&anchor_url, ctx->wc_ctx, anchor_abspath,
                                   scratch_pool, scratch_pool));
      SVN_ERR_ASSERT(anchor_url != NULL);

      target_url = NULL;
    }
  else /* is_copy && revision2->kind == svn_opt_revision_base */
    {
#if 0
      svn_node_kind_t kind;
#endif
      /* ### Ugly hack ahead ###
       *
       * We're diffing a locally copied/moved node.
       * Describe the copy source to the reporter instead of the copy itself.
       * Doing the latter would generate a single add_directory() call to the
       * diff editor which results in an unexpected diff (the copy would
       * be shown as deleted).
       *
       * ### But if we will receive any real changes from the repositor we
       * will most likely fail to apply them as the wc diff editor assumes
       * that we have the data to which the change applies in BASE...
       */

      target_url = svn_path_url_add_component2(cf_repos_root_url,
                                               cf_repos_relpath,
                                               scratch_pool);

#if 0
      /*SVN_ERR(svn_wc_read_kind2(&kind, ctx->wc_ctx, abspath2, FALSE, FALSE,
                                scratch_pool));

      if (kind != svn_node_dir
          || strcmp(copy_root_abspath, abspath2) != 0) */
#endif
          /* We are looking at a subdirectory of the repository,
             We can describe the parent directory as the anchor..

             ### This 'appears to work', but that is really dumb luck
             ### for the simple cases in the test suite */
          anchor_abspath = svn_dirent_dirname(abspath2, scratch_pool);
          anchor_url = svn_path_url_add_component2(cf_repos_root_url,
                                                   svn_relpath_dirname(
                                                            cf_repos_relpath,
                                                            scratch_pool),
                                                   scratch_pool);
          target = svn_dirent_basename(abspath2, NULL);
          anchor = svn_dirent_dirname(path2, scratch_pool);
#if 0
          /* This code, while ok can't be enabled without causing test
           * failures. The repository will send some changes against
           * BASE for nodes that don't have BASE...
           */
          anchor_abspath = abspath2;
          anchor_url = svn_path_url_add_component2(cf_repos_root_url,
                                                   cf_repos_relpath,
                                                   scratch_pool);
          anchor = path2;
          target = "";
#endif
  SVN_ERR(svn_ra_reparent(ra_session, anchor_url, scratch_pool));
  if (ddi)
    {
      const char *repos_root_url;

      ddi->anchor = anchor;

      if (!reverse)
        {
          ddi->orig_path_1 = apr_pstrdup(result_pool, loc1->url);
          ddi->orig_path_2 =
            svn_path_url_add_component2(anchor_url, target, result_pool);
        }
      else
        {
          ddi->orig_path_1 =
            svn_path_url_add_component2(anchor_url, target, result_pool);
          ddi->orig_path_2 = apr_pstrdup(result_pool, loc1->url);
        }

      SVN_ERR(svn_ra_get_repos_root2(ra_session, &repos_root_url,
                                      scratch_pool));

      ddi->session_relpath = svn_uri_skip_ancestor(repos_root_url,
                                                   anchor_url,
                                                   result_pool);
    }

  if (reverse)
    diff_processor = svn_diff__tree_processor_reverse_create(
                              diff_processor, NULL, scratch_pool);
                                SVN_RA_CAPABILITY_DEPTH, scratch_pool));
                                  ignore_ancestry,
                                  diff_processor,
                                  scratch_pool, scratch_pool));
  if (is_copy && revision2->kind != svn_opt_revision_base)
    {
                              loc1->rev,
                              loc1->url,
                              diff_editor, diff_edit_baton,
                              scratch_pool));
      if (cf_depth == svn_depth_unknown)
        cf_depth = svn_depth_infinity;
      /* Reporting the in-wc revision as r0, makes the repository send
         everything as added, which avoids using BASE for pristine information,
         which is not there (or unrelated) for a copy */
                                 ignore_ancestry ? 0 : cf_revision,
                                 cf_depth, FALSE, NULL, scratch_pool));
      if (*target)
                                    target_url,
                                    ignore_ancestry ? 0 : cf_revision,
                                    cf_depth, FALSE, NULL, scratch_pool));
      SVN_ERR(reporter->finish_report(reporter_baton, scratch_pool));
                              loc1->rev,
                              loc1->url,
                              diff_editor, diff_edit_baton,
                              scratch_pool));
                                      scratch_pool));
/* This is basically just the guts of svn_client_diff[_summarize][_peg]6(). */
do_diff(const char **root_relpath,
        svn_boolean_t *root_is_dir,
        diff_driver_info_t *ddi,
        svn_boolean_t text_deltas,
        const svn_diff_tree_processor_t *diff_processor,
        svn_client_ctx_t *ctx,
        apr_pool_t *result_pool,
        apr_pool_t *scratch_pool)
          SVN_ERR(diff_repos_repos(root_relpath, root_is_dir,
                                   ddi,
                                   text_deltas,
                                   diff_processor, ctx,
                                   result_pool, scratch_pool));
          SVN_ERR(diff_repos_wc(root_relpath, root_is_dir, ddi,
                                path_or_url1, revision1, peg_revision,
                                ignore_ancestry, changelists,
                                diff_processor, ctx,
                                result_pool, scratch_pool));
          SVN_ERR(diff_repos_wc(root_relpath, root_is_dir, ddi,
                                path_or_url2, revision2, peg_revision,
                                ignore_ancestry, changelists,
                                diff_processor, ctx,
                                result_pool, scratch_pool));
              SVN_ERR(svn_dirent_get_absolute(&abspath1, path_or_url1,
                                              scratch_pool));
              SVN_ERR(svn_dirent_get_absolute(&abspath2, path_or_url2, 
                                              scratch_pool));
              /* ### What about ddi? */

              SVN_ERR(svn_client__arbitrary_nodes_diff(root_relpath, root_is_dir,
                                                       abspath1, abspath2,
                                                       diff_processor,
                                                       ctx,
                                                       result_pool, scratch_pool));
            {
              SVN_ERR(diff_wc_wc(root_relpath, root_is_dir, ddi,
                                 path_or_url1, revision1,
                                 path_or_url2, revision2,
                                 depth, ignore_ancestry, changelists,
                                 diff_processor, ctx,
                                 result_pool, scratch_pool));
            }
/* Initialize DWI.diff_cmd and DWI.options,
 * Allocate the fields in RESULT_POOL, which should be at least as long-lived
 * as the pool DWI itself is allocated in.
create_diff_writer_info(diff_writer_info_t *dwi,
                        const apr_array_header_t *options,
                        apr_hash_t *config, apr_pool_t *result_pool)
            options = svn_cstring_split(diff_extensions, " \t\n\r", TRUE,
                                        result_pool);
    options = apr_array_make(result_pool, 0, sizeof(const char *));
    SVN_ERR(svn_path_cstring_to_utf8(&dwi->diff_cmd, diff_cmd,
                                     result_pool));
    dwi->diff_cmd = NULL;
  if (dwi->diff_cmd)
          argv = apr_palloc(result_pool, argc * sizeof(char *));
                      APR_ARRAY_IDX(options, i, const char *), result_pool));
      dwi->options.for_external.argv = argv;
      dwi->options.for_external.argc = argc;
      dwi->options.for_internal = svn_diff_file_options_create(result_pool);
      SVN_ERR(svn_diff_file_options_parse(dwi->options.for_internal,
                                          options, result_pool));
  diff_writer_info_t dwi = { 0 };
  const svn_diff_tree_processor_t *diff_processor;
  svn_diff_tree_processor_t *processor;
  dwi.ddi.orig_path_1 = path_or_url1;
  dwi.ddi.orig_path_2 = path_or_url2;

  SVN_ERR(create_diff_writer_info(&dwi, options,
                                  ctx->config, pool));
  dwi.pool = pool;
  dwi.outstream = outstream;
  dwi.errstream = errstream;
  dwi.header_encoding = header_encoding;

  dwi.force_binary = ignore_content_type;
  dwi.ignore_properties = ignore_properties;
  dwi.properties_only = properties_only;
  dwi.relative_to_dir = relative_to_dir;
  dwi.use_git_diff_format = use_git_diff_format;
  dwi.no_diff_added = no_diff_added;
  dwi.no_diff_deleted = no_diff_deleted;
  dwi.show_copies_as_adds = show_copies_as_adds;

  dwi.cancel_func = ctx->cancel_func;
  dwi.cancel_baton = ctx->cancel_baton;

  dwi.wc_ctx = ctx->wc_ctx;
  dwi.ddi.session_relpath = NULL;
  dwi.ddi.anchor = NULL;

  processor = svn_diff__tree_processor_create(&dwi, pool);

  processor->dir_added = diff_dir_added;
  processor->dir_changed = diff_dir_changed;
  processor->dir_deleted = diff_dir_deleted;

  processor->file_added = diff_file_added;
  processor->file_changed = diff_file_changed;
  processor->file_deleted = diff_file_deleted;

  diff_processor = processor;

  /* --show-copies-as-adds and --git imply --notice-ancestry */
  if (show_copies_as_adds || use_git_diff_format)
    ignore_ancestry = FALSE;

  return svn_error_trace(do_diff(NULL, NULL, &dwi.ddi,
                                 path_or_url1, path_or_url2,
                                 revision1, revision2, &peg_revision,
                                 depth, ignore_ancestry, changelists,
                                 TRUE /* text_deltas */,
                                 diff_processor, ctx, pool, pool));
  diff_writer_info_t dwi = { 0 };
  const svn_diff_tree_processor_t *diff_processor;
  svn_diff_tree_processor_t *processor;
  dwi.ddi.orig_path_1 = path_or_url;
  dwi.ddi.orig_path_2 = path_or_url;

  SVN_ERR(create_diff_writer_info(&dwi, options,
                                  ctx->config, pool));
  dwi.pool = pool;
  dwi.outstream = outstream;
  dwi.errstream = errstream;
  dwi.header_encoding = header_encoding;

  dwi.force_binary = ignore_content_type;
  dwi.ignore_properties = ignore_properties;
  dwi.properties_only = properties_only;
  dwi.relative_to_dir = relative_to_dir;
  dwi.use_git_diff_format = use_git_diff_format;
  dwi.no_diff_added = no_diff_added;
  dwi.no_diff_deleted = no_diff_deleted;
  dwi.show_copies_as_adds = show_copies_as_adds;

  dwi.cancel_func = ctx->cancel_func;
  dwi.cancel_baton = ctx->cancel_baton;

  dwi.wc_ctx = ctx->wc_ctx;
  dwi.ddi.session_relpath = NULL;
  dwi.ddi.anchor = NULL;

  processor = svn_diff__tree_processor_create(&dwi, pool);

  processor->dir_added = diff_dir_added;
  processor->dir_changed = diff_dir_changed;
  processor->dir_deleted = diff_dir_deleted;

  processor->file_added = diff_file_added;
  processor->file_changed = diff_file_changed;
  processor->file_deleted = diff_file_deleted;

  diff_processor = processor;

  /* --show-copies-as-adds and --git imply --notice-ancestry */
  if (show_copies_as_adds || use_git_diff_format)
    ignore_ancestry = FALSE;

  return svn_error_trace(do_diff(NULL, NULL, &dwi.ddi,
                                 path_or_url, path_or_url,
                                 start_revision, end_revision, peg_revision,
                                 depth, ignore_ancestry, changelists,
                                 TRUE /* text_deltas */,
                                 diff_processor, ctx, pool, pool));
  const svn_diff_tree_processor_t *diff_processor;
  const char **p_root_relpath;

  /* We will never do a pegged diff from here. */
  SVN_ERR(svn_client__get_diff_summarize_callbacks(
                     &diff_processor, &p_root_relpath,
                     summarize_func, summarize_baton,
                     path_or_url1, pool, pool));

  return svn_error_trace(do_diff(p_root_relpath, NULL, NULL,
                                 path_or_url1, path_or_url2,
                                 revision1, revision2, &peg_revision,
                                 depth, ignore_ancestry, changelists,
                                 FALSE /* text_deltas */,
                                 diff_processor, ctx, pool, pool));
  const svn_diff_tree_processor_t *diff_processor;
  const char **p_root_relpath;
  SVN_ERR(svn_client__get_diff_summarize_callbacks(
                     &diff_processor, &p_root_relpath,
                     summarize_func, summarize_baton,
                     path_or_url, pool, pool));

  return svn_error_trace(do_diff(p_root_relpath, NULL, NULL,
                                 path_or_url, path_or_url,
                                 start_revision, end_revision, peg_revision,
                                 depth, ignore_ancestry, changelists,
                                 FALSE /* text_deltas */,
                                 diff_processor, ctx, pool, pool));
