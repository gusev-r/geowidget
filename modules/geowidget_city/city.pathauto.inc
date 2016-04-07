<?php

/**
 * @file
 * Pathauto integration for geowidget_city module.
 *
 * @ingroup pathauto
 */

/**
 * Batch processing callback; Generate aliases for citys.
 */
function city_bulk_update_batch_process(&$context) {

  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('geowidget_city', 'p');
  $query->leftJoin('url_alias', 'ua', "CONCAT('city/', p.id) = ua.source");
  $query->addField('p', 'id');
  $query->isNull('ua.source');
  $query->condition('p.id', $context['sandbox']['current'], '>');
  $query->orderBy('p.id');
  $query->addTag('pathauto_bulk_update');
  $query->addMetaData('entity', 'city');
  // Получаем количество продуктов для обработки.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // Если нет продуктов для обработки, то тут же завершаем работу.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $ids = $query->execute()->fetchCol();

  _city_update_alias_multiple($ids, 'bulkupdate');
  $context['sandbox']['count'] += count($ids);
  $context['sandbox']['current'] = max($ids);
  $context['message'] = t('Updated alias for city @id.', array('@id' => end($ids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
}