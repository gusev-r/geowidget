<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class GeowidgetCityMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {

    $info = parent::entityPropertyInfo();

    $properties = &$info[$this->type]['properties'];

    $properties['edit_url'] = array(
      'label' => t('Edit URL'),
      'description' => t("The URL of the message's edit page."),
      'getter callback' => 'entity_metadata_geowidgetcity_get_properties',
      'type' => 'uri',
      'computed' => TRUE, // поле рассчитывается "на лету".
      'entity views field' => TRUE, // для Views
    );
    $properties['delete_url'] = array(
      'label' => t('Delete URL'),
      'description' => t("The URL of the message's edit page."),
      'getter callback' => 'entity_metadata_geowidgetcity_get_properties',
      'type' => 'uri',
      'computed' => TRUE, //
      'entity views field' => TRUE,
    );
    $properties['title'] = array(
      'label' => t('Title'),
      'description' => t("The title of the message."),
      'getter callback' => 'entity_metadata_geowidgetcity_get_properties',
      'type' => 'text',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );
    $properties['parent_city'] = array(
      'label' => t("Parent city"),
      'type' => 'city',
      'description' => t("The title of the message."),
      'getter callback' => 'entity_metadata_geowidgetcity_get_properties',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );
    return $info;
  }

}
